#include "token.hpp"

#include <cctype>
#include <unordered_map>

TokenType scan_token_type(const std::string_view &token) {
    if (!token.empty() && std::isdigit(static_cast<unsigned char>(token.front()))) {
        return TokenType::NUMERIC_LITERAL;
    }
    static const std::unordered_map<std::string_view, TokenType> token_map = {
        // Types
        {"int", TokenType::INT},
        {"float", TokenType::FLOAT},
        {"string", TokenType::STRING},
        {"dynamic", TokenType::DYNAMIC},
        {"bool", TokenType::BOOL},
        {"none", TokenType::NONE},
        // Special sequences
        {"->", TokenType::RETURN_MARKER},
        {":", TokenType::TYPE_MARKER},
        {"::", TokenType::SCOPE_RESOLUTION},

        {"(", TokenType::L_BRACKET},
        {")", TokenType::R_BRACKET},
        {"{", TokenType::L_BRACE},
        {"}", TokenType::R_BRACE},
        {",", TokenType::COMMA},

        {";", TokenType::LINE_END},

        {"+", TokenType::ADD},
        {"+=", TokenType::ADD_ASSIGN},
        {"-", TokenType::SUBTRACT},
        {"*", TokenType::MULTIPLY},
        {"/", TokenType::DIVIDE},
        {"=", TokenType::ASSIGN},

        {"if", TokenType::IF},
        {"else", TokenType::ELSE},
        {"function", TokenType::FUNCTION},
        {"return", TokenType::RETURN}};
    const auto result = token_map.find(token);
    return result == token_map.end() ? TokenType::IDENTIFIER : result->second;
}
