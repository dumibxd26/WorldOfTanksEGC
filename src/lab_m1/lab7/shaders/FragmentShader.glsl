#version 330

// Input
// TODO(student): Get color value from vertex shader
in vec3 color;


// Output
layout(location = 0) out vec4 out_color;


uniform float HP;

void main()
{
    // TODO(student): Write pixel out color
    float real_HP = HP;
    if (real_HP > 3)
     real_HP = 3;

    if (real_HP < 0)
	 real_HP = 0;

    vec3 new_color = color - (vec3(0.15, 0.15, 0.15) - vec3(0.05 * real_HP, 0.05 * real_HP, 0.05 * real_HP));


    out_color = vec4(new_color, 1);
}