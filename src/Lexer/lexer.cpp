// The main file for the lexer implementation.

#include "lexer.hpp"

#include <cctype>
#include <stdexcept>

[[noreturn]] void lexer::_raise_error(const std::string_view message, const int line) const {
    throw std::runtime_error("Lex error on line " + std::to_string(line) + ": " +
                             std::string(message));
}

bool lexer::is_alpha(const char &c) {
    return (std::isalpha(static_cast<unsigned char>(c)) || c == '_');
}

bool lexer::is_alphanumeric(const char &c) {
    return (is_alpha(c) || std::isdigit(static_cast<unsigned char>(c)));
}

bool lexer::is_special_character(const char &c) {
    return (c == '(' || c == ')' || c == '{' || c == '}' || c == ',' || c == ';' || c == ':' ||
            c == '+' || c == '-' || c == '*' || c == '/' || c == '=');
}

bool lexer::is_special_sequence(const std::string_view &sequence) {
    return (sequence == "->" || sequence == "::" || sequence == "+=");
}

void lexer::_skip_whitespace_and_comments(std::size_t &position, int &line) {
    while (not _is_at_end(position)) {
        const char current = _peek(position);

        if (current == '\n') {
            ++line;
            ++position;
            continue;
        }

        if (std::isspace(static_cast<unsigned char>(current))) {
            ++position;
            continue;
        }

        if (current == '/' && not _is_at_end(position + 1) && _peek(position + 1) == '/') {
            position += 2;
            while (not _is_at_end(position) && _peek(position) != '\n') {
                ++position;
            }
            continue;
        }

        break;
    }
}

void lexer::_consume_string_literal(std::size_t &position, int &line) {
    const std::size_t start = position;
    const int start_line = line;
    ++position;

    while (not _is_at_end(position)) {
        const char current = _peek(position);

        if (current == '"') {
            ++position;
            tokens.push_back(
                Token(TokenType::STRING_LITERAL, input.substr(start, position - start), start_line));
            return;
        }

        if (current == '\\') {
            ++position;
            if (_is_at_end(position)) {
                _raise_error("unterminated escape sequence", line);
            }

            const char escaped = _peek(position);
            if (escaped != '"' && escaped != '\\' && escaped != 'n' && escaped != 'r' &&
                escaped != 't') {
                _raise_error("unsupported escape sequence", line);
            }
            ++position;
            continue;
        }

        if (current == '\n') {
            ++line;
        }
        ++position;
    }

    _raise_error("unterminated string literal", start_line);
}

void lexer::_consume_numeric_literal(std::size_t &position, const int line) {
    do {
        ++position;
    } while (not _is_at_end(position) && std::isdigit(static_cast<unsigned char>(_peek(position))));
    if (not _is_at_end(position) && _peek(position) == '.') {
        ++position;
        while (not _is_at_end(position) &&
               std::isdigit(static_cast<unsigned char>(_peek(position)))) {
            ++position;
        }
    }

    if (not _is_at_end(position) && is_alpha(_peek(position))) {
        _raise_error("identifiers cannot begin with a number", line);
    }
}

void lexer::_consume_alphanumeric(std::size_t &position) {
    do {
        ++position;
    } while (not _is_at_end(position) && is_alphanumeric(_peek(position)));
}

void lexer::_consume_special_sequence_or_operator(std::size_t &position) {
    if (not _is_at_end(position + 1) &&
        is_special_sequence(std::string_view(input.data() + position, 2))) {
        position += 2;
        return;
    }

    ++position;
}

void lexer::_consume_docstring(std::size_t &position, int &line) {
    const int start_line = line;
    if (not _is_at_end(position + 2) && _peek(position) == '"' && _peek(position + 1) == '"' &&
        _peek(position + 2) == '"') {
        position += 3;
    } else {
        return;
    }
    while (not _is_at_end(position + 2) &&
           !(input[position] == '"' && input[position + 1] == '"' && input[position + 2] == '"')) {
        if (_peek(position) == '\n') {
            ++line;
        }
        ++position;
    }
    if (_is_at_end(position + 2)) {
        _raise_error("unterminated docstring", start_line);
    }

    position += 3;
}

std::vector<Token> lexer::lex() {
    std::size_t position = 0;
    int line = 1;
    tokens.clear();

    while (not _is_at_end(position)) {
        const char current = _peek(position);
        // Although the if statements below are not strictly necessary and are technically
        // duplicated, they improve readability for little cost.

        if (std::isspace(static_cast<unsigned char>(current))) {
            _skip_whitespace_and_comments(position, line);
            continue;
        }

        if (current == '/' && not _is_at_end(position + 1) && _peek(position + 1) == '/') {
            _skip_whitespace_and_comments(position, line);
            continue;
        }

        if (current == '"') {
            if (not _is_at_end(position + 2) && _peek(position + 1) == '"' &&
                _peek(position + 2) == '"') {
                _consume_docstring(position, line);
            } else {
                _consume_string_literal(position, line);
            }
            continue;
        }

        const std::size_t start = position;

        if (is_alpha(current)) {
            _consume_alphanumeric(position);
        } else if (std::isdigit(static_cast<unsigned char>(current))) {
            _consume_numeric_literal(position, line);
        } else if (is_special_character(current)) {
            _consume_special_sequence_or_operator(position);
        } else {
            ++position;
            _raise_error("invalid character", line);
        }

        const std::string lexeme = input.substr(start, position - start);
        tokens.push_back(_generate_token(lexeme, line));
    }

    tokens.push_back(Token(TokenType::EOF_TOKEN, "", line));
    return tokens;
};
