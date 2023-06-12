#include <vull/ui/Tree.hh>

#include <vull/core/Input.hh>
#include <vull/maths/Vec.hh>
#include <vull/support/Assert.hh>
#include <vull/support/Optional.hh>
#include <vull/support/UniquePtr.hh>
#include <vull/support/Utility.hh>
#include <vull/ui/Element.hh>
#include <vull/ui/Event.hh>
#include <vull/ui/layout/Pane.hh>

namespace vull::ui {

void Tree::render(Painter &painter) {
    if (m_root_element->is_pane()) {
        static_cast<Pane &>(*m_root_element).layout();
    }
    if (vull::exchange(m_need_hover_update, false)) {
        update_hover();
    }
    m_root_element->paint(painter, m_root_element->offset_in_parent());
}

void Tree::update_hover() {
    auto hit_result = m_root_element->hit_test(m_mouse_position - m_root_element->offset_in_parent());
    if (!hit_result) {
        // Nothing hovered.
        if (m_hovered_element) {
            MouseEvent exit_event(m_hovered_relative_position, m_mouse_buttons);
            m_hovered_element->handle_mouse_exit(exit_event);
        }
        m_hovered_element = {};
        return;
    }

    // Update mouse position relative to the currently hovered element.
    m_hovered_relative_position = hit_result->relative_position;

    auto &hovered_element = hit_result->element;
    if (&hovered_element == m_hovered_element.ptr()) {
        // Same element hovered.
        return;
    }

    MouseEvent enter_exit_event(m_hovered_relative_position, m_mouse_buttons);
    if (m_hovered_element) {
        m_hovered_element->handle_mouse_exit(enter_exit_event);
    }
    m_hovered_element = hovered_element;
    if (!m_active_element) {
        // Only send mouse enter if an element isn't hijacking input events. This prevents a button from being
        // highlighted from hovering when dragging a slider, for example.
        m_hovered_element->handle_mouse_enter(enter_exit_event);
    }
}

void Tree::handle_element_destruct(Element &element) {
    if (m_active_element.ptr() == &element) {
        m_active_element = {};
        m_need_hover_update = true;
    }
    if (m_hovered_element.ptr() == &element) {
        m_hovered_element = {};
        m_need_hover_update = true;
    }
}

void Tree::set_active_element(Element &element) {
    VULL_ASSERT(!m_active_element);
    m_active_element = element;
}

void Tree::unset_active_element() {
    m_active_element = {};
    m_need_hover_update = true;
}

// Helper for calculating a position relative to an element given a global screen position. For example, there may be an
// active element hijacking input events. This means that the mouse may be outside the active element but still
// interacting with it (e.g. moving a slider). We still need to calculate a position for the mouse relative to the
// active element to pass to the event handler.
static Vec2f calculate_element_relative_position(Optional<Element &> element, Vec2f global_position) {
    Vec2f relative_position = global_position;
    for (; element; element = element->parent()) {
        relative_position -= element->offset_in_parent();
    }
    return relative_position;
}

template <auto event_fn>
static void dispatch_event(Optional<Element &> element, const auto &event) {
    for (; element; element = element->parent()) {
        if ((*element.*event_fn)(event)) {
            return;
        }
    }
}

template <auto event_fn>
void Tree::handle_mouse_press_release(MouseButton button) {
    if (!m_active_element) {
        // No active element hijacking events, just send the event to the hovered element if present.
        MouseButtonEvent event(m_hovered_relative_position, m_mouse_buttons, button);
        dispatch_event<event_fn>(m_hovered_element, event);
        return;
    }

    // Otherwise there is an active element hijacking input events.
    const auto relative_position = calculate_element_relative_position(m_active_element, m_mouse_position);
    MouseButtonEvent event(relative_position, m_mouse_buttons, button);
    dispatch_event<event_fn>(m_active_element, event);
}

void Tree::handle_mouse_press(MouseButton button) {
    handle_mouse_press_release<&Element::handle_mouse_press>(button);
}

void Tree::handle_mouse_release(MouseButton button) {
    handle_mouse_press_release<&Element::handle_mouse_release>(button);
}

void Tree::handle_mouse_move(Vec2f delta, Vec2f position, MouseButtonMask buttons) {
    delta /= m_global_scale;
    m_mouse_position = position / m_global_scale;
    m_mouse_buttons = buttons;

    // Update the currently hovered element.
    update_hover();

    // TODO: Should mouse move events propagate? (use dispatch_event)
    if (!m_active_element) {
        // No active element hijacking events, just send the move to the hovered element if present.
        if (m_hovered_element) {
            MouseMoveEvent move_event(m_hovered_relative_position, m_mouse_buttons, delta);
            m_hovered_element->handle_mouse_move(move_event);
        }
        return;
    }

    // Otherwise there is an active element hijacking move events.
    const auto relative_position = calculate_element_relative_position(m_active_element, m_mouse_position);
    MouseMoveEvent move_event(relative_position, buttons, delta);
    m_active_element->handle_mouse_move(move_event);
}

} // namespace vull::ui
