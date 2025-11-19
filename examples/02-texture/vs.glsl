#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Color;
layout (location = 2) in vec2 a_UV;

out vec3 v_Color;
out vec2 v_UV;

void main(void) { 
    gl_Position = vec4(a_Position, 1.0);
    v_Color = a_Color;
    v_UV = a_UV;
}
