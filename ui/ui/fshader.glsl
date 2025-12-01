#version 430

in vec2 v_uv;
in vec3 v_position;
in vec3 v_normal;
in float height;

out vec4 fragColor;

void main() {
   vec3 color = vec3(0.39, 1.0, 0.0);
   fragColor = vec4(1.0);
}
