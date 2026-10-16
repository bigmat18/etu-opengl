#ifndef RAY_MARCHING
#define RAY_MARCHING

#define MAX_STEPS 100
#define MINIMUM_HIT_DISTANCE 0.001
#define MAXIMUM_TRACE_DISTANCE 1000.0
 
// Compute distance in ray marching
float rm_dist(mat4 inv_model, vec3 pos, float type) {
    vec3 transformed_position = vec3(inv_model * vec4(pos, 1.0));
    float dist_local = calculate_distance(transformed_position, 
                                          vec3(0.0), 1.0, 1.0, type);

    vec3 sX = inv_model[0].xyz;
    vec3 sY = inv_model[1].xyz;
    vec3 sZ = inv_model[2].xyz;            
    float max_axis = max(length(sX), max(length(sY), length(sZ)));

    dist_local *= (1.0/max_axis);
    return dist_local;
}

// Compute final color (with diffuse) in ray marching
vec4 rm_color(mat4 inv_model, vec3 pos, float type, vec3 color) {
    vec3 transformed_position = vec3(inv_model * vec4(pos, 1.0));
    vec3 object_space_normal = calculate_normal(transformed_position, 
                                                1.0, 1.0, type);

    vec3 world_space_normal = normalize(transpose(mat3(inv_model)) * object_space_normal);
    vec3 light_dir = normalize(LIGHT_POS - pos);
    vec3 diffuse = compute_diffuse(world_space_normal, light_dir, LIGHT_COLOR);

    return vec4((diffuse + 0.3) * color, 1.0);
}

#endif
