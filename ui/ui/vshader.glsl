#version 430

layout(location = 0) in vec4 vertex;
layout(location = 1) in vec2 coords; // uv
layout(location = 2) in vec3 normal;

out vec2 v_uv;
out vec3 v_position;
out vec3 v_normal;
out float height;

// Use uniform names expected by Mesh::renderForward()
uniform mat4 MVP;
uniform mat4 Model;

// Heightmap
uniform sampler2D heightmap;

void main() {
    // Transform position with the model matrix and pass it to the fragment shader
    v_uv = coords;
    height = texture(heightmap, coords).r;
    v_position = (Model * (vertex + vec4(0, height, 0, 0))).xyz;


    float eps = 0.01;
    float hx1 = texture(heightmap, coords + vec2(eps, 0.0)).r;
    float hy1 = texture(heightmap, coords + vec2(0.0, eps)).r;
    float hx2 = texture(heightmap, coords - vec2(eps, 0.0)).r;
    float hy2 = texture(heightmap, coords - vec2(0.0, eps)).r;

    vec3 dx = vec3(2.0*eps, hx1 - hx2, 0.0);
    vec3 dy = vec3(0.0, hy1 - hy2, 2.0*eps);

    v_normal = normalize(cross(dy , dx));

    // Final clip-space position
    gl_Position = MVP * vec4(v_position, 1.0);
}
