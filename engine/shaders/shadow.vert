#version 460
#include "lib/common.glsl"
#include "lib/object.glsl"
#include "lib/ubo.glsl"

layout (location = 0) in vec3 g_position;

DECLARE_UBO(0);

void main() {
    gl_Position = g_shadow_info.cascade_matrices[g_cascade_index] * object_transform() * vec4(g_position, 1.0f);
}
