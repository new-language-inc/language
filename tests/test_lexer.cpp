#include "../src/Lexer/lexer.hpp"
#include <cassert>

void test_lexer() {
    lexer lex("int x = 5;");
    auto tokens = lex.lex();

    assert(tokens.size() == 6);
    assert(tokens[0].type == TokenType::INT);
    assert(tokens[1].type == TokenType::IDENTIFIER);
    assert(tokens[2].type == TokenType::ASSIGN);
    assert(tokens[3].type == TokenType::NUMERIC_LITERAL);
    assert(tokens[4].type == TokenType::LINE_END);
    assert(tokens[5].type == TokenType::EOF_TOKEN);
}