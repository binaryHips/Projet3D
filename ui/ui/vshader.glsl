#version 430

layout(location = 0) in vec4 vertex;
layout(location = 1) in vec2 coords; // uv
layout(location = 2) in vec3 normal;

out vec2 v_uv;
out vec3 v_position;
out vec3 v_normal;
out float height;
out int material;

// Use uniform names expected by Mesh::renderForward()
uniform mat4 MVP;
uniform mat4 Model;

// Heightmap
uniform sampler2D heightmapBedrock;
uniform sampler2D heightmapStone;
uniform sampler2D heightmapSand;
uniform sampler2D heightmapWater;


float getHeightAt(vec2 pos, out int material){
    float vBedrock = texture(heightmapBedrock, pos);
    float vStone = texture(heightmapStone, pos);
    float vSand = texture(heightmapSand, pos);
    float vWater = texture(heightmapWater, pos);

    // dependent on structure
    if (vWater > 0.0 || vSand > 0.0){
        material =  (vWater > vSand) ? 3 : 2;
    } else{
        material = vStone > 0.0;
    }

    return vBedrock+ vStone + max(vSand + vWater);
}

void main() {
    // Transform position with the model matrix and pass it to the fragment shader
    v_uv = coords;
    height = getHeightAt(coords, material);
    v_position = (Model * (vertex + vec4(0, height, 0, 0))).xyz;

    // Merci Killian pour l'idée tye le sang

    float eps = 0.001;
    float hx1 = getHeightAt(coords + vec2(eps, 0.0)).r;
    float hy1 = getHeightAt(coords + vec2(0.0, eps)).r;
    float hx2 = getHeightAt(coords - vec2(eps, 0.0)).r;
    float hy2 = getHeightAt(coords - vec2(0.0, eps)).r;

    vec3 dx = vec3(2.0*eps, hx1 - hx2, 0.0);
    vec3 dy = vec3(0.0, hy1 - hy2, 2.0*eps);

    v_normal = normalize(cross(dy , dx));

    // Final clip-space position
    gl_Position = MVP * vec4(v_position, 1.0);
}
