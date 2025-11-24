#ifndef PHONG_MODEL
#define PHONG_MODEL

vec3 compute_diffuse(vec3 nnorm, vec3 frag_pos, 
                     vec3 light_dir, vec3 light_color) 
{
    return max(dot(light_dir, nnorm), 0.0) * light_color;
}

vec3 compute_specular(vec3 nnorm, vec3 frag_pos,
                      vec3 view_pos, vec3 light_dir, 
                      vec3 light_color, float shininess) 
{
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, nnorm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), shininess);
    return spec * light_color;
}

#endif // PHONG_MODEL
