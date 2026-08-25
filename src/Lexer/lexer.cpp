// The main file for the lexer implementation.

#include "lexer.hpp"

#include <cctype>

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
    ++position;

    while (not _is_at_end(position) && _peek(position) != '"') {
        if (_peek(position) == '\n') {
            ++line;
        }
        ++position;
    }

    // TODO: raise an error if the closing quote is missing.
    if (not _is_at_end(position)) {
        ++position;
    }
}

void lexer::_consume_numeric_literal(std::size_t &position) {
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
            _consume_string_literal(position, line);
            continue;
        }

        const std::size_t start = position;

        if (is_alpha(current)) {
            _consume_alphanumeric(position);
        } else if (std::isdigit(static_cast<unsigned char>(current))) {
            _consume_numeric_literal(position);
        } else if (is_special_character(current)) {
            _consume_special_sequence_or_operator(position);
        } else {
            ++position;
            // TODO: raise an invalid character error here.
        }

        const std::string lexeme = input.substr(start, position - start);
        tokens.push_back(_generate_token(lexeme, line));
    }

    tokens.push_back(Token(TokenType::EOF_TOKEN, "", line));
    return tokens;
};
