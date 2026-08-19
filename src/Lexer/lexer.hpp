#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <string_view>
#include <vector>

#include "token.hpp"

class lexer {
  private:
    std::string input;

    std::vector<Token> tokens;

    bool _is_at_end(const std::size_t &position) const { return position >= input.length(); }

    char _peek(const std::size_t &position) const {
        return position < input.length() ? input[position] : '\0';
    }

    void _skip_whitespace_and_comments(std::size_t &position, int &line);
    void _consume_string_literal(std::size_t &position, int &line);
    void _consume_alpha(std::size_t &position);
    void _consume_numeric(std::size_t &position);
    void _consume_special_sequence_or_operator(std::size_t &position);

    Token _generate_token(const std::string &lexeme, const int &line) {
        return Token(scan_token_type(lexeme), lexeme, line);
    }

  public:
    lexer(std::string input) { this->input = input; }

    std::vector<Token> lex();
    bool is_alpha(const char &c);
    bool is_alphanumeric(const char &c);
    bool is_special_character(const char &c);
    bool is_special_sequence(const std::string_view &sequence);
};

#endif
