#version 330 core  
out vec4 out_FragColor;

in vec3 v_Normal;

void main(void) { 
    out_FragColor = vec4(v_Normal, 1.0);
}
