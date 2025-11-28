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

    // Final clip-space position
    gl_Position = MVP * vec4(v_position, 1.0);
}
