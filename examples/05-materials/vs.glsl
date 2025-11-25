#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec4 a_Tangent;
layout (location = 3) in vec2 a_UV;

out vec3 v_FragPos;
out mat3 v_TBN;
out vec2 v_UV;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform vec3 u_ViewPos;

void main(void) { 
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);

    vec3 T = normalize(vec3(u_Model * vec4(a_Tangent.xyz, 0.0)));
    vec3 N = normalize(vec3(u_Model * vec4(a_Normal,      0.0)));
    vec3 B = normalize(cross(N, T) * a_Tangent.w);

    v_UV = a_UV;
    v_FragPos = vec3(u_Model * vec4(a_Position, 1.0));
    v_TBN = mat3(T, B, N);
}
