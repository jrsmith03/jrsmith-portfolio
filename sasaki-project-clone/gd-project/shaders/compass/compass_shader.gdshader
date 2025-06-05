shader_type spatial;

uniform float hue_speed = 1.0;    // How fast the hue cycles
uniform float swirl_speed = 1.0;  // How fast the UV distortion animates
uniform float swirl_scale = 0.5;  // How strong the swirl distortion is

// HSV to RGB conversion
vec3 hsv2rgb(vec3 c) {
    vec3 rgb = clamp(abs(mod(c.x*6.0 + vec3(0.0,4.0,2.0), 6.0)-3.0)-1.0,
                     0.0, 1.0);
    rgb = rgb*rgb*(3.0-2.0*rgb); // Gamma correction curve
    return c.z * mix(vec3(1.0), rgb, c.y);
}

void fragment() {
    vec2 uv = UV;

    // Time-based swirling distortion on UVs
    float t = TIME * swirl_speed;
    float waveX = sin(uv.y * 10.0 + t) * swirl_scale;
    float waveY = cos(uv.x * 10.0 + t) * swirl_scale;
    uv += vec2(waveX, waveY);

    // Wrap UV for continuous effect (may require a repeating texture)
    uv = fract(uv);

    // Cyclic hue shift over time
    float hue = fract(TIME * hue_speed);
    float saturation = 1.0;
    float value = 1.0;
    vec3 base_color = hsv2rgb(vec3(hue, saturation, value));

    // Additional pattern to enhance trippy look
    float rad = length(uv - 0.5);
    float fade = sin(rad * 10.0 + t) * 0.5 + 0.5;
    vec3 final_color = base_color * fade;

    ALBEDO = final_color;

    // Optional: Add emission for a glowing, neon look
    EMISSION = final_color * 0.5;
    ROUGHNESS = 0.2;
}
