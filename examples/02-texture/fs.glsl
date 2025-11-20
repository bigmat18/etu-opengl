#version 330 core  
out vec4 FragColor;

in vec3 v_Color;
in vec2 v_UV;

uniform sampler2D u_Texture;

void main(void) { 
    FragColor = mix(texture(u_Texture, v_UV), vec4(v_Color, 1.0), 0.5);
}
