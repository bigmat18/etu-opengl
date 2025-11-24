#ifndef MATERIALS_UNIFORMS
#define MATERIALS_UNIFORMS

uniform vec3        u_Ambient;
uniform vec3        u_Diffuse;
uniform vec3        u_Specular;
uniform vec3        u_Transmittance;
uniform vec3        u_Emission;

uniform float       u_Ior;
uniform float       u_Shininess;
uniform float       u_Dissolve;
uniform float       u_Illum;

uniform sampler2D   u_TexDiff;
uniform sampler2D   u_TexSpec;
uniform sampler2D   u_TexNorm; 
uniform sampler2D   u_TexAmbient;
uniform sampler2D   u_TexRoughness;
uniform sampler2D   u_TexEmissive;

#define LIGHT_POS   vec3(1.2f, 1.0f, 2.0f)
#define LIGHT_COLOR vec3(1.0f, 1.0f, 1.0f)

#endif // MATERIALS_UNIFORMS
