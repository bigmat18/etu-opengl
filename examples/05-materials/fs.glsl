#version 330 core  
#include <etugl/uniforms.glsl>
#include <etugl/phong_model.glsl>

out vec4 o_FragColor;
uniform vec3 u_Position;

in vec3 v_Normal;
in vec2 v_UV;
in vec3 v_FragPos;

void main(void) { 
    vec3 ambient = vec3(0.1);
    vec3 nnorm = normalize(v_Normal);
    vec3 light_dir = normalize(LIGHT_POS - v_FragPos);

    vec3 diffuse = compute_diffuse(v_Normal, v_FragPos, 
                                   light_dir, LIGHT_COLOR);

    vec3 specular = compute_specular(v_Normal, v_FragPos, 
                                     u_Position, light_dir, 
                                     LIGHT_COLOR, u_Shininess);

    diffuse = (diffuse + ambient) * vec3(texture(u_TexDiff, v_UV));
    specular = specular * texture(u_TexSpec, v_UV).x;

    o_FragColor = vec4(diffuse + specular, 1.0);
}
