#version 330 core  
out vec4 FragColor;

in vec3 v_Normal;
in vec2 v_UV;

uniform sampler2D u_Texture;
uniform vec3 u_Diffuse;

void main(void) { 
    FragColor = texture(u_Texture, v_UV);

    //FragColor = vec4(v_UV, 0.0, 1.0);
    //FragColor = vec4(v_Normal, 1.0);
}
