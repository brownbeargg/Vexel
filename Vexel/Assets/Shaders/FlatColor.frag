#version 450 core

layout(location = 0) out vec4 FragColor;

layout(location = 1) in vec3 v_Color;

void main()
{
    FragColor = vec4(v_Color, 1.0);
}
