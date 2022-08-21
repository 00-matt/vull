layout (constant_id = 0) const float k_viewport_width = 0;
layout (constant_id = 1) const float k_viewport_height = 0;

struct UiObject {
    vec4 colour;
    vec2 position;
    vec2 scale;
    uint glyph_index;
    uint type;
};
