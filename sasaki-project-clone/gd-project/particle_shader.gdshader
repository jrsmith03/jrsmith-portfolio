shader_type particles;

// The base color of the particle
uniform vec4 base_color;

// Controls the scaling of the particle over its lifetime
uniform float particle_scale : hint_range(0.1, 10.0);

void vertex() {
    // The initial particle position
    vec3 final_position = VELOCITY;

    // Scale the particle over its lifetime
    final_position *= mix(1.0, particle_scale, LIFETIME / LIFETIME_MAX);

    // Set the final transformed position
    VELOCITY = final_position;
}

void fragment() {
    // Output the base color as the particle color
    COLOR = base_color;
}
