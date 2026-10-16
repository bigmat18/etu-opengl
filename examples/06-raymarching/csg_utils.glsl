#ifndef CSG_UTILS
#define CSG_UTILS

// SDF for generic sphere
float distance_from_sphere(vec3 p, vec3 center, float radius) {
	return length(p - center) - radius;
}

// SDF for generic cube
float distance_from_cube(vec3 p, vec3 center, float side) {
    vec3 q = abs(p - center) - (side/2.0);
    return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

// SDF for generic cylinder
float distance_from_cylinder(vec3 p, vec3 center, float radius, float height) {
    vec2 d = abs(vec2(length(p.xz - center.xz), p.y - center.y)) - vec2(radius, height / 2.0);
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0));
}

// Generic distance function
// p: origin point
// c: center of the shape
// l: length parameter (radius for sphere, half-side for cube)
// type: 
//  - 0: sphere
//  - 1: cube
//  - 2: cylinder
float calculate_distance(vec3 p, vec3 c, float l, float h, float type) {
    if (type == 0.0)
        return distance_from_sphere(p, c, l);
    if (type == 1.0)
        return distance_from_cube(p, c, l);
    if (type == 2.0)
        return distance_from_cylinder(p, c, l, h);
}

// Map the world to SDF
float map_the_world(vec3 p, float l, float h, float type) {
    return calculate_distance(p, vec3(0.0), l, h, type);
}

// Calculate normal at point p with given shape type
vec3 calculate_normal(vec3 p, float l, float h, float type) {
    const vec3 small_step = vec3(0.001, 0.0, 0.0);

    float gradient_x = map_the_world(p + small_step.xyy, l, h, type) - map_the_world(p - small_step.xyy, l, h, type);
    float gradient_y = map_the_world(p + small_step.yxy, l, h, type) - map_the_world(p - small_step.yxy, l, h, type);
    float gradient_z = map_the_world(p + small_step.yyx, l, h, type) - map_the_world(p - small_step.yyx, l, h, type);

    return normalize(vec3(gradient_x, gradient_y, gradient_z));
}


#endif // CSG_UTILS
