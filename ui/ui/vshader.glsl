#version 430

layout(location = 0) in vec4 vertex;
layout(location = 1) in vec2 coords; // uv
layout(location = 2) in vec3 normal;

out vec3 v_position;
out vec3 v_normal;

// Use uniform names expected by Mesh::renderForward()
uniform mat4 MVP;
uniform mat4 Model;

// Heightmap
uniform sampler2D heightmap;

void main() {
    // Transform position with the model matrix and pass it to the fragment shader
    vec4 height = texture(heightmap, coords);
    v_position = (Model * vertex).xyz + height.y;


    // Compute normal matrix on the GPU (transpose(inverse(mat3(Model))))
    mat3 normal_matrix = transpose(inverse(mat3(Model)));
    v_normal = normal_matrix * normal;

    // Final clip-space position
    gl_Position = MVP * vertex;
}
