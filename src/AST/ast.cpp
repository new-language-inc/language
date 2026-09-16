#include "ast.hpp"

#include <utility>

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
