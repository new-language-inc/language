// The main file for the lexer implementation.

#include <string>
#include <vector>

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

    TokenType _scan_token_type(const std::string &token) {
        if (token == "int") {
            return TokenType::INT;
        } else if (token == "float") {
            return TokenType::FLOAT;
        } else if (token == "string") {
            return TokenType::STRING;
        } else if (token == "(") {
            return TokenType::L_BRACKET;
        } else if (token == ")") {
            return TokenType::R_BRACKET;
        } else if (token == ";") {
            line += 1;
            return TokenType::LINE_END;
        } else {
            return TokenType::IDENTIFIER;
        }
    }

    Token _generate_token(const std::string &token) {
        return Token(_scan_token_type(token), token, line);
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

int main() { return 0; }
