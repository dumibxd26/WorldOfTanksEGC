#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

uniform float HP;

// Output value to fragment shader
out vec3 color;

// Function to generate Perlin noise
float rand(vec2 co){
    return fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
    // Compute world space vectors
    vec3 world_position = vec3(Model * vec4(v_position, 1));
    vec3 world_normal = normalize(vec3(Model * vec4(v_normal, 0)));

    vec3 L = normalize(light_position - world_position);
    vec3 V = normalize(eye_position - world_position);
    vec3 H = normalize(L + V);
    vec3 R = normalize(reflect(-L, world_normal));

    // Define ambient light component
    float ambient_light = 0.25;

    // Compute diffuse light component
    float diffuse_light = material_kd * max(dot(world_normal, L), 0);

    // Compute specular light component
    float specular_light = 0;

    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Gouraud (1971) shading method. There is also the Phong (1975) shading
    // method, which we'll use in the future. Don't mix them up!
    if (diffuse_light > 0)
    {
         specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);
    }

    // Compute light
    float factorAtenuare = 1.0 / (1.0 + 0.14 * distance(light_position, world_position) + 0.07 * distance(light_position, world_position) * distance(light_position, world_position));
    float light = ambient_light + factorAtenuare * (diffuse_light + specular_light);

    // USED MODIFIED PERLIN NOISE BY ME 

    // Modify noise intensity based on HP (clamped between 0 and 3)

    float noise = (0.03 - HP / 100);

    if (HP < 1) {
		noise = 0.03;
	}

    if (HP > 3) {
        noise = 0;
    }

    vec3 deformedPosition = v_position + noise * vec3(
        rand(v_position.xy),
        rand(v_position.yz),
        rand(v_position.zx)
    );

    // Compute world space vectors using the deformed position
    vec3 deformedWorldPosition = vec3(Model * vec4(deformedPosition, 1));

    // Compute light with deformed position
    float deformedFactorAtenuare = 1.0 / (1.0 + 0.14 * distance(light_position, deformedWorldPosition) + 0.07 * distance(light_position, deformedWorldPosition) * distance(light_position, deformedWorldPosition));
    float deformedLight = ambient_light + deformedFactorAtenuare * (diffuse_light + specular_light);

    // Send color light output to fragment shader
    color = object_color * deformedLight;

    gl_Position = Projection * View * Model * vec4(deformedPosition, 1.0);
}
