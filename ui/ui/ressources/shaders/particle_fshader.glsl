#version 430

out vec4 fragColor;

uniform vec4 particleColor;

void main() {
    // // Create circular particle (discard corners of the point sprite)
    // vec2 coord = gl_PointCoord - vec2(0.5);
    // if (length(coord) > 0.5)
    //     discard;
    
    // // Soft edge
    // float alpha = 1.0 - smoothstep(0.3, 0.5, length(coord));
    // fragColor = vec4(particleColor.rgb, particleColor.a * alpha);

    fragColor = particleColor;
}
