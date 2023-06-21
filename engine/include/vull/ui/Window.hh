#pragma once

#include <vull/support/Optional.hh>
#include <vull/support/String.hh>
#include <vull/ui/Units.hh> // IWYU pragma: keep
#include <vull/ui/layout/Pane.hh>

namespace vull::ui {

class Painter;
class Element;
class MouseButtonEvent;
class MouseMoveEvent;
class Tree;

class Window final : public Pane {
    Pane *m_title_pane;
    Pane *m_content_pane;
    bool m_is_resizing{false};

public:
    Window(Tree &tree, Optional<Element &> parent, String title);

    // ^Element
    void paint(Painter &painter, LayoutPoint position) const override;
    bool handle_mouse_press(const MouseButtonEvent &event) override;
    bool handle_mouse_release(const MouseButtonEvent &event) override;
    void handle_mouse_move(const MouseMoveEvent &event) override;

    // ^Pane
    void pre_layout(LayoutSize available_space) override;
    void layout(LayoutSize available_space) override;

    Pane &title_pane() { return *m_title_pane; }
    Pane &content_pane() { return *m_content_pane; }
};

} // namespace vull::ui
