// The main file for the lexer implementation.

#include <string>
#include <vector>

#include "token.hpp"

class Lexer {
  private:
    std::string input;

    std::vector<Token> tokens;

    std::string next_word;
    Token token;

    int position = 0;
    int line = 1;

    std::string _get_next_word() {
        int start = position;
        while (_is_at_end() && !isspace(input[position])) {
            position++;
        }
        return input.substr(start, position - start);
    }

    bool _is_at_end() {
        if (position < input.size()) {
            return true;
        } else {
            return false;
        }
    }

    Token _generate_token(const std::string &token) {
        return Token(scan_token_type(token), token, line);
    }

  public:
    Lexer(std::string input) { this->input = input; }

    std::vector<Token> parse() {
        while (not _is_at_end()) {
            next_word = _get_next_word();
            token = _generate_token(next_word);
            tokens.push_back(token);
        }
        tokens.push_back(Token(TokenType::EOF_TOKEN, "", 0));
        return tokens;
    }
};
