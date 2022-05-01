#version 460

layout (location = 0) in vec3 g_position;

layout (binding = 0) uniform UniformBuffer {
    mat4 proj;
    mat4 view;
    mat4 sun_matrix;
    vec3 camera_position;
} g_ubo;

layout (push_constant) uniform ObjectData {
    mat4 g_transform;
};

void main() {
    gl_Position = g_ubo.sun_matrix * g_transform * vec4(g_position, 1.0f);
}
