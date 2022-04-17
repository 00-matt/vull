#pragma once

#include "../Ast.hh"
#include "Builder.hh"

#include <vull/support/Vector.hh>

namespace spv {

class Backend : public ast::Visitor {
    Builder m_builder;
    Function *m_function{nullptr};
    Block *m_block{nullptr};
    vull::Vector<Id> m_expr_stack;

    // Vertex shader.
    Id m_position_output{0};
    bool m_is_vertex_entry{false};

    Id convert_type(ast::ScalarType);

public:
    void visit(const ast::Block &) override;
    void visit(const ast::ConstantList &) override;
    void visit(const ast::Function &) override;
    void visit(const ast::ReturnStmt &) override;

    const Builder &builder() const { return m_builder; }
};

} // namespace spv
