#include "ast.hpp"

#include <stdexcept>
#include <utility>

namespace {
void require_token_type(const Token &token, const TokenType expected, const char *factory) {
    if (token.type != expected) {
        throw std::invalid_argument(std::string(factory) + " received an unexpected token type");
    }
}

bool is_type_token(const TokenType type) {
    return type == TokenType::INT || type == TokenType::FLOAT || type == TokenType::STRING ||
           type == TokenType::DYNAMIC || type == TokenType::BOOL || type == TokenType::NONE;
}
} // namespace

number_expr::number_expr(const double value) : value(value) {}

string_expr::string_expr(std::string value) : value(std::move(value)) {}

bool_expr::bool_expr(const bool value) : value(value) {}

identifier_expr::identifier_expr(std::string name) : name(std::move(name)) {}

binary_expr::binary_expr(expr_ptr left, const TokenType op, expr_ptr right)
    : left(std::move(left)), op(op), right(std::move(right)) {}

call_expr::call_expr(expr_ptr callee, std::vector<expr_ptr> arguments)
    : callee(std::move(callee)), arguments(std::move(arguments)) {}

assignment_expr::assignment_expr(std::string name, const TokenType op, expr_ptr value)
    : name(std::move(name)), op(op), value(std::move(value)) {}

expression_stmt::expression_stmt(expr_ptr expression) : expression(std::move(expression)) {}

variable_decl_stmt::variable_decl_stmt(const TokenType type, std::string name, expr_ptr initializer)
    : type(type), name(std::move(name)), initializer(std::move(initializer)) {}

block_stmt::block_stmt(std::vector<stmt_ptr> statements) : statements(std::move(statements)) {}

if_stmt::if_stmt(expr_ptr condition, std::unique_ptr<block_stmt> then_branch,
                 std::unique_ptr<block_stmt> else_branch)
    : condition(std::move(condition)), then_branch(std::move(then_branch)),
      else_branch(std::move(else_branch)) {}

return_stmt::return_stmt(expr_ptr value) : value(std::move(value)) {}

function_stmt::function_stmt(std::string name, std::vector<parameter> parameters,
                             const TokenType return_type, std::unique_ptr<block_stmt> body)
    : name(std::move(name)), parameters(std::move(parameters)), return_type(return_type),
      body(std::move(body)) {}

program::program(std::vector<stmt_ptr> statements) : statements(std::move(statements)) {}

void ast::log_error(const char *str, int line) const {
    throw std::runtime_error(std::string("Error at line ") + std::to_string(line) + ": " + str);
}

std::unique_ptr<bool_expr> ast::parse_bool_expr(const Token &token) {
    if (token.type != TokenType::TRUE && token.type != TokenType::FALSE) {
        throw std::invalid_argument("parse_bool_expr requires a boolean-literal token");
    }
    return std::make_unique<bool_expr>(token.type == TokenType::TRUE);
}

std::unique_ptr<identifier_expr> ast::parse_identifier_expr(const Token &token) {
    require_token_type(token, TokenType::IDENTIFIER, "parse_identifier_expr");
    return std::make_unique<identifier_expr>(token.lexeme);
}

std::unique_ptr<none_expr> ast::parse_none_expr(const Token &token) {
    require_token_type(token, TokenType::NONE, "parse_none_expr");
    return std::make_unique<none_expr>();
}

std::unique_ptr<binary_expr> ast::parse_binary_expr(expr_ptr left, const Token &op_token,
                                                    expr_ptr right) {
    if (op_token.type != TokenType::ADD && op_token.type != TokenType::SUBTRACT &&
        op_token.type != TokenType::MULTIPLY && op_token.type != TokenType::DIVIDE) {
        throw std::invalid_argument("parse_binary_expr requires an arithmetic operator token");
    }
    return std::make_unique<binary_expr>(std::move(left), op_token.type, std::move(right));
}

std::unique_ptr<call_expr> ast::parse_call_expr(expr_ptr callee, std::vector<expr_ptr> arguments) {
    return std::make_unique<call_expr>(std::move(callee), std::move(arguments));
}

std::unique_ptr<assignment_expr> ast::parse_assignment_expr(const Token &name_token,
                                                            const Token &op_token, expr_ptr value) {
    require_token_type(name_token, TokenType::IDENTIFIER, "parse_assignment_expr");
    if (op_token.type != TokenType::ASSIGN && op_token.type != TokenType::ADD_ASSIGN) {
        throw std::invalid_argument("parse_assignment_expr requires an assignment operator token");
    }
    return std::make_unique<assignment_expr>(name_token.lexeme, op_token.type, std::move(value));
}

std::unique_ptr<expression_stmt> ast::parse_expression_stmt(expr_ptr expression) {
    return std::make_unique<expression_stmt>(std::move(expression));
}

std::unique_ptr<variable_decl_stmt> ast::parse_variable_decl_stmt(const Token &type_token,
                                                                  const Token &name_token,
                                                                  expr_ptr initializer) {
    if (!is_type_token(type_token.type)) {
        throw std::invalid_argument("parse_variable_decl_stmt requires a type token");
    }
    require_token_type(name_token, TokenType::IDENTIFIER, "parse_variable_decl_stmt");
    return std::make_unique<variable_decl_stmt>(type_token.type, name_token.lexeme,
                                                std::move(initializer));
}

std::unique_ptr<block_stmt> ast::parse_block_stmt(std::vector<stmt_ptr> statements) {
    return std::make_unique<block_stmt>(std::move(statements));
}

std::unique_ptr<if_stmt> ast::parse_if_stmt(expr_ptr condition,
                                            std::unique_ptr<block_stmt> then_branch,
                                            std::unique_ptr<block_stmt> else_branch) {
    return std::make_unique<if_stmt>(std::move(condition), std::move(then_branch),
                                     std::move(else_branch));
}

std::unique_ptr<return_stmt> ast::parse_return_stmt(expr_ptr value) {
    return std::make_unique<return_stmt>(std::move(value));
}

std::unique_ptr<function_stmt> ast::parse_function_stmt(const Token &name_token,
                                                        std::vector<parameter> parameters,
                                                        const Token &return_type_token,
                                                        std::unique_ptr<block_stmt> body) {
    require_token_type(name_token, TokenType::IDENTIFIER, "parse_function_stmt");
    if (!is_type_token(return_type_token.type)) {
        throw std::invalid_argument("parse_function_stmt requires a return type token");
    }
    return std::make_unique<function_stmt>(name_token.lexeme, std::move(parameters),
                                           return_type_token.type, std::move(body));
}

std::unique_ptr<number_expr> ast::parse_number_expr(const Token &token) {
    if (token.type != TokenType::NUMERIC_LITERAL) {
        throw std::invalid_argument("parse_number_expr requires a numeric-literal token");
    }

    return std::make_unique<number_expr>(std::stod(token.lexeme));
}

std::unique_ptr<string_expr> ast::parse_string_expr(const Token &token) {
    if (token.type != TokenType::STRING_LITERAL) {
        throw std::invalid_argument("parse_string_expr requires a string-literal token");
    }

    return std::make_unique<string_expr>(token.lexeme);
}
