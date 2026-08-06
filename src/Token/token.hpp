#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <string_view>

enum class TokenType {
    IDENTIFIER,
    L_BRACKET,
    R_BRACKET,
    INT,
    FLOAT,
    STRING,
    OPERATOR,
    PUNCTUATOR,
    LINE_END,
    EOF_TOKEN,
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
};

TokenType scan_token_type(const std::string_view &token);

#endif