#include "../src/Lexer/lexer.hpp"

#include <cassert>
#include <stdexcept>

void test_lexer() {
    lexer boolean_literals{"true false"};
    const auto boolean_tokens = boolean_literals.lex();
    assert(boolean_tokens[0].type == TokenType::TRUE);
    assert(boolean_tokens[1].type == TokenType::FALSE);

    lexer lex("int x = 5;");
    auto tokens = lex.lex();

    assert(tokens.size() == 6);
    assert(tokens[0].type == TokenType::INT);
    assert(tokens[1].type == TokenType::IDENTIFIER);
    assert(tokens[2].type == TokenType::ASSIGN);
    assert(tokens[3].type == TokenType::NUMERIC_LITERAL);
    assert(tokens[4].type == TokenType::LINE_END);
    assert(tokens[5].type == TokenType::EOF_TOKEN);

    lexer escaped{"string message = \"hello\\n\\\"world\";"};
    const auto escaped_tokens = escaped.lex();
    assert(escaped_tokens[3].type == TokenType::STRING_LITERAL);
    assert(escaped_tokens[3].lexeme == "\"hello\\n\\\"world\"");

    lexer docstring{R"("""documentation""" int x = 1;)"};
    const auto docstring_tokens = docstring.lex();
    assert(docstring_tokens[0].type == TokenType::INT);

    try {
        lexer invalid_character{"@"};
        invalid_character.lex();
        assert(false);
    } catch (const std::runtime_error &) {
    }

    try {
        lexer unterminated_string{"\"unfinished"};
        unterminated_string.lex();
        assert(false);
    } catch (const std::runtime_error &) {
    }
}
