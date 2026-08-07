#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>

#include "token.hpp"

class lexer {
  private:
    std::string input;

    std::vector<Token> tokens;

    std::string lexeme;
    Token token;

    bool _is_at_end(const int &position) { return (position >= input.length()); }

    char _peek(const int &position) { return input[position + 1]; }

    Token _generate_token(const std::string &lexeme, const int &line) {
        return Token(scan_token_type(lexeme), lexeme, line);
    }

  public:
    lexer(std::string input) { this->input = input; }

    std::vector<Token> lex();
    bool _is_alpha(const char &c);
    bool _is_alphanumeric(const char &c);
};

#endif