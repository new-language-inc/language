// The main file for the lexer implementation.

#include "lexer.hpp"

#include <cctype>

bool lexer::is_alpha(const char &c) { return (isalpha(c) || c == '_'); }

bool lexer::is_alphanumeric(const char &c) { return (is_alpha(c) || isdigit(c)); }

bool lexer::is_special_character(const char &c) {
    return (c == '(' || c == ')' || c == ';' || c == ':' || c == '-' || c == '>' || c == '/');
}

bool lexer::is_special_sequence(const std::string &sequence) {
    return (sequence == "->" || sequence == "::");
}

std::vector<Token> lexer::lex() {
    int position = 0;
    int line = 1;
    tokens.clear();
    while (not _is_at_end(position)) {
        const char current = input[position];

        switch (current) {
        case ' ':
        case '\t':
        case '\r':
        case '\v':
        case '\f':
        case '\n':
            if (current == '\n') {
                line++;
            }
            do {
                ++position;
            } while (not _is_at_end(position) &&
                     std::isspace(static_cast<unsigned char>(input[position])));
            continue;
        case '/':
            if (not _is_at_end(position + 1) && input[position + 1] == '/') {
                position += 2;
                while (not _is_at_end(position) && input[position] != '\n') {
                    ++position;
                }
                continue;
            }
            break;
        default:
            break;
        }

        const int start = position;
        if (is_alphanumeric(current)) {
            do {
                ++position;
            } while (not _is_at_end(position) && is_alphanumeric(input[position]));
        } else if (is_special_character(current)) {
            if (is_special_sequence(input.substr(position, 2))) {
                position += 2;
            } else {
                ++position;
            }
        } else {
            ++position;
        }

        const std::string lexeme = input.substr(start, position - start);
        tokens.push_back(_generate_token(lexeme, line));
    }
    tokens.push_back(Token(TokenType::EOF_TOKEN, "", 0));
    return tokens;
};
