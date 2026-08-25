#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "token.hpp"

class lexer {
  private:
    std::string input;

    std::vector<Token> tokens;

    bool _is_at_end(std::size_t position) const { return position >= input.length(); }

    char _peek(std::size_t position) const {
        return position < input.length() ? input[position] : '\0';
    }

    [[noreturn]] void _raise_error(std::string_view message, int line) const;
    void _skip_whitespace_and_comments(std::size_t &position, int &line);
    void _consume_string_literal(std::size_t &position, int &line);
    void _consume_docstring(std::size_t &position, int &line);
    void _consume_alphanumeric(std::size_t &position);
    void _consume_numeric_literal(std::size_t &position, int line);
    void _consume_special_sequence_or_operator(std::size_t &position);

    Token _generate_token(const std::string &lexeme, int line) {
        return Token(scan_token_type(lexeme), lexeme, line);
    }

  public:
    explicit lexer(std::string input) : input(std::move(input)) {}

    std::vector<Token> lex();
    bool is_alpha(const char &c);
    bool is_alphanumeric(const char &c);
    bool is_special_character(const char &c);
    bool is_special_sequence(const std::string_view &sequence);
};

#endif
