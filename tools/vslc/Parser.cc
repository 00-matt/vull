#include "Parser.hh"

#include <vull/support/Assert.hh>

#include "Lexer.hh"
#include "Token.hh"

namespace {

ast::Type parse_type(const Token &ident) {
    if (ident.string() == "float") {
        return {ast::ScalarType::Float, 1};
    }
    if (ident.string() == "vec2") {
        return {ast::ScalarType::Float, 2};
    }
    if (ident.string() == "vec3") {
        return {ast::ScalarType::Float, 3};
    }
    if (ident.string() == "vec4") {
        return {ast::ScalarType::Float, 4};
    }
    VULL_ENSURE_NOT_REACHED();
}

} // namespace

vull::Optional<Token> Parser::consume(TokenKind kind) {
    const auto &token = m_lexer.peek();
    return token.kind() == kind ? m_lexer.next() : vull::Optional<Token>();
}

Token Parser::expect(TokenKind kind) {
    auto token = m_lexer.next();
    VULL_ENSURE(token.kind() == kind);
    return token;
}

ast::Constant *Parser::parse_constant() {
    if (auto literal = consume(TokenKind::FloatLit)) {
        return m_root.allocate<ast::Constant>(literal->decimal());
    }
    if (auto literal = consume(TokenKind::IntLit)) {
        return m_root.allocate<ast::Constant>(literal->integer());
    }
    VULL_ENSURE_NOT_REACHED();
}

ast::Node *Parser::parse_expr() {
    if (auto ident = consume(TokenKind::Ident)) {
        if (!consume(TokenKind::LeftParen)) {
            return m_root.allocate<ast::Symbol>(ident->string());
        }
        auto *construct_expr = m_root.allocate<ast::Aggregate>(ast::AggregateKind::ConstructExpr);
        construct_expr->set_type(parse_type(*ident));
        while (!consume(TokenKind::RightParen)) {
            construct_expr->append_node(parse_expr());
            consume(TokenKind::Comma);
        }
        return construct_expr;
    }
    return parse_constant();
}

ast::Node *Parser::parse_stmt() {
    auto *expr = parse_expr();
    // Implicit return.
    return m_root.allocate<ast::ReturnStmt>(expr);
}

ast::Aggregate *Parser::parse_block() {
    expect(TokenKind::LeftBrace);
    auto *block = m_root.allocate<ast::Aggregate>(ast::AggregateKind::Block);
    while (!consume(TokenKind::RightBrace)) {
        block->append_node(parse_stmt());
    }
    return block;
}

ast::Function *Parser::parse_function() {
    auto name = expect(TokenKind::Ident);
    expect(TokenKind::LeftParen);

    vull::Vector<ast::Parameter> parameters;
    while (!consume(TokenKind::RightParen)) {
        expect(TokenKind::KeywordLet);
        auto param_name = expect(TokenKind::Ident);
        expect(TokenKind::Colon);
        auto type = parse_type(expect(TokenKind::Ident));
        parameters.emplace(param_name.string(), type);
        consume(TokenKind::Comma);
    }

    expect(TokenKind::Colon);
    auto return_type = parse_type(expect(TokenKind::Ident));
    auto *block = parse_block();
    return m_root.allocate<ast::Function>(name.string(), block, return_type, vull::move(parameters));
}

ast::Node *Parser::parse_top_level() {
    switch (m_lexer.next().kind()) {
    case TokenKind::KeywordFn:
        return parse_function();
    default:
        VULL_ENSURE_NOT_REACHED();
    }
}

ast::Root Parser::parse() {
    while (!consume(TokenKind::Eof)) {
        m_root.append_top_level(parse_top_level());
    }
    return vull::move(m_root);
}
