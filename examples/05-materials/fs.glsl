#version 330 core  
out vec4 out_FragColor;

in vec3 v_Normal;
in vec2 v_UV;

uniform sampler2D u_TexDiff;
uniform sampler2D u_TexNorm;

void main(void) { 
    out_FragColor = texture(u_TexNorm, v_UV);
}
