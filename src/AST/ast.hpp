#ifndef AST_HPP
#define AST_HPP

#include <memory>
#include <string>
#include <vector>

#include "token.hpp"

// The common base class for every AST node.
class ast_node {
  public:
    virtual ~ast_node() = default;
};

class expr : public ast_node {
  public:
    ~expr() override = default;
};

using expr_ptr = std::unique_ptr<expr>;

class number_expr : public expr {
  public:
    explicit number_expr(double value);

    double value;
};

class string_expr : public expr {
  public:
    explicit string_expr(std::string value);

    std::string value;
};

class bool_expr : public expr {
  public:
    explicit bool_expr(bool value);

    bool value;
};

class none_expr : public expr {};

class identifier_expr : public expr {
  public:
    explicit identifier_expr(std::string name);

    std::string name;
};

class binary_expr : public expr {
  public:
    binary_expr(expr_ptr left, TokenType op, expr_ptr right);

    expr_ptr left;
    TokenType op;
    expr_ptr right;
};

class call_expr : public expr {
  public:
    call_expr(expr_ptr callee, std::vector<expr_ptr> arguments);

    expr_ptr callee;
    std::vector<expr_ptr> arguments;
};

class assignment_expr : public expr {
  public:
    assignment_expr(std::string name, TokenType op, expr_ptr value);

    std::string name;
    TokenType op;
    expr_ptr value;
};

class stmt : public ast_node {
  public:
    ~stmt() override = default;
};

using stmt_ptr = std::unique_ptr<stmt>;

class expression_stmt : public stmt {
  public:
    explicit expression_stmt(expr_ptr expression);

    expr_ptr expression;
};

class variable_decl_stmt : public stmt {
  public:
    variable_decl_stmt(TokenType type, std::string name, expr_ptr initializer);

    TokenType type;
    std::string name;
    expr_ptr initializer;
};

class block_stmt : public stmt {
  public:
    explicit block_stmt(std::vector<stmt_ptr> statements);

    std::vector<stmt_ptr> statements;
};

class if_stmt : public stmt {
  public:
    if_stmt(expr_ptr condition, std::unique_ptr<block_stmt> then_branch,
            std::unique_ptr<block_stmt> else_branch = nullptr);

    expr_ptr condition;
    std::unique_ptr<block_stmt> then_branch;
    std::unique_ptr<block_stmt> else_branch;
};

class return_stmt : public stmt {
  public:
    explicit return_stmt(expr_ptr value = nullptr);

    expr_ptr value;
};

struct parameter {
    TokenType type;
    std::string name;
};

class function_stmt : public stmt {
  public:
    function_stmt(std::string name, std::vector<parameter> parameters, TokenType return_type,
                  std::unique_ptr<block_stmt> body);

    std::string name;
    std::vector<parameter> parameters;
    TokenType return_type;
    std::unique_ptr<block_stmt> body;
};

class program : public stmt {
  public:
    explicit program(std::vector<stmt_ptr> statements);

    std::vector<stmt_ptr> statements;
};

#endif // AST_HPP
