#version 330 core  
out vec4 o_FragColor;

#include <etugl/phong_model.glsl>

#include <uniforms.glsl>
#include <csg_utils.glsl>
#include <ray_marching.glsl>


void main(void) {  
    vec2 uv = ((v_UV * 2.0) - 1.0) * (u_Resolution / u_Resolution.y); 

    vec3 ray_origin = u_ViewPos;
    vec3 ray_direction = normalize(transpose(mat3(u_View)) * vec3(uv, -1.0));

    vec4 result = vec4(0.0);        // final color 
    float total_distance = 0.0;     // traveled distance

    for (int i = 0; i < MAX_STEPS; i++) {
        vec3 current_position = ray_origin + (ray_direction * total_distance);

        // Light rendering
        float light_dist = calculate_distance(current_position, LIGHT_POS, 1.0, 1.0, 0);
        if (light_dist < MINIMUM_HIT_DISTANCE) {
            result = vec4(LIGHT_COLOR, 1.0);
            break;
        }

        // Model rendering
        float min_scene_dist = light_dist; 
        int candidate_idx = -1;
        for (int j = 0; j < NUM_PRIMITIVES; j++) {
            float dist = rm_dist(u_Primitives[j].inv_model, current_position, u_Primitives[j].type);
            if (dist < min_scene_dist) {
                min_scene_dist = dist;
                candidate_idx = j;
            }
        }

        if (min_scene_dist < MINIMUM_HIT_DISTANCE) {
            result = rm_color(u_Primitives[candidate_idx].inv_model, 
                              current_position, 
                              u_Primitives[candidate_idx].type,
                              u_Primitives[candidate_idx].color.xyz);
            break;
        }

        if (total_distance > MAXIMUM_TRACE_DISTANCE)
            break;

        total_distance += min_scene_dist;
    }

    if (result.w != 0.0) o_FragColor = result;
    else                 discard;
}
