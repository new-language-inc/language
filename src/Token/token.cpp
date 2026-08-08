#include "token.hpp"

#include <unordered_map>

TokenType scan_token_type(const std::string_view &token) {
    static const std::unordered_map<std::string_view, TokenType> token_map = {
        {"int", TokenType::INT},       {"float", TokenType::FLOAT},
        {"string", TokenType::STRING}, {"->", TokenType::RETURN_MARKER},
        {":", TokenType::TYPE_MARKER}, {"::", TokenType::SCOPE_RESOLUTION},
        {"(", TokenType::L_BRACKET},   {")", TokenType::R_BRACKET},
        {";", TokenType::LINE_END}};
    if (token_map.contains(token)) {
        return token_map.at(token);
    } else {
        return TokenType::IDENTIFIER;
    }
}