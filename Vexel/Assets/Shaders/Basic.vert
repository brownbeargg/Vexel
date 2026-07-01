#version 450 core

layout(binding = 0) uniform UniformBufferObject
{
    mat4 Model;
    mat4 View;
    mat4 Proj;
}
ubo;

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;

layout(location = 0) out vec3 v_Color;

void main()
{
    v_Color = a_Color;

    gl_Position = ubo.Proj * ubo.View * ubo.Model * vec4(a_Position, 1.0);
}
