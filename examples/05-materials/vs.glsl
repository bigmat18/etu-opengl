#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_UV;

out vec3 v_FragPos;
out vec3 v_Normal;
out vec2 v_UV;

uniform vec3 u_Position;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;


void main(void) { 
    mat3 normalModel = mat3(transpose(inverse(u_Model)));

    v_FragPos = vec3(u_Model * vec4(a_Position, 1.0));
    v_Normal = normalModel * a_Normal;
    v_UV = a_UV;

    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}
