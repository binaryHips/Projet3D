#version 430

out vec4 fragColor;

uniform vec4 particleColor;
in vec3 globalPointPosition;
void main() {
    // // Create circular particle (discard corners of the point sprite)

    
    // don't show particles too far away
    if (length(globalPointPosition - vec3(0.5, 0.0, 0.5)) > 3.0){
        discard;
    }
    fragColor = particleColor;
}
