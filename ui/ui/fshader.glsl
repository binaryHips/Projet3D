#version 430

in vec2 v_uv;
in vec3 v_position;
in vec3 v_normal;
in float height;


vec3 light_position = vec3(0,2,2);

out vec4 fragColor;

void main() {
//   vec3 color = vec3(0.39, 1.0, 0.0);
//   fragColor = vec4(1.0);

    vec3 L = normalize(light_position - v_position);
    float NL = max(dot(normalize(v_normal), L), 0.0);
    vec3 color = vec3(0.39, 1.0, 0.0);
    vec3 col = clamp(color * 0.2 + color * 0.8 * NL, 0.0, 1.0);
    fragColor = vec4(col, 1.0);
}
