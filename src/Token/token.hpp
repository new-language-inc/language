#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <string_view>

enum class TokenType {
    IDENTIFIER,

    STRING_LITERAL,
    NUMERIC_LITERAL,

    L_BRACKET,
    R_BRACKET,
    L_BRACE,
    R_BRACE,
    COMMA,
    // Types
    INT,
    FLOAT,
    STRING,
    DYNAMIC,
    BOOL,
    NONE,

    // Operators
    ADD,
    ADD_ASSIGN,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    ASSIGN,

    // Keywords
    IF,
    ELSE,
    FUNCTION,
    RETURN,

    // Special sequences
    RETURN_MARKER,
    TYPE_MARKER,
    SCOPE_RESOLUTION,
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
