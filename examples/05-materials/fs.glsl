#version 330 core  
#include <etugl/uniforms.glsl>
#include <etugl/phong_model.glsl>

out vec4 o_FragColor;
uniform vec3 u_ViewPos;

in vec3 v_FragPos;
in mat3 v_TBN;
in vec2 v_UV;

#define AMBIENT vec3(0.1)

void main(void) { 
    vec3 normal = texture(u_TexNorm, v_UV).rgb;
    normal = normal * 2.0 - 1.0;
    normal = normalize(v_TBN * normal);

    vec3 light_dir = normalize(LIGHT_POS - v_FragPos);
    vec3 view_dir  = normalize(u_ViewPos - v_FragPos);

    vec3 diffuse = compute_diffuse(normal, light_dir, LIGHT_COLOR);
    vec3 specular = compute_specular(normal, view_dir, light_dir, LIGHT_COLOR, u_Shininess);

    diffuse = (diffuse + AMBIENT) * vec3(texture(u_TexDiff, v_UV));
    specular = specular * texture(u_TexSpec, v_UV).x;

    o_FragColor = vec4(diffuse + specular, 1.0);
}
