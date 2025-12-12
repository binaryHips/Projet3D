#version 430

in vec2 v_uv;
in vec3 v_position;
in vec3 v_normal;
in float height;
flat in int material;

vec3 light_position = vec3(0,2,2);

out vec4 fragColor;


struct MaterialReturn {
    vec3 color;
    // in case more needed
};


// materials

MaterialReturn undefined(){
    MaterialReturn res;
    vec3 L = normalize(light_position - v_position);
    float NL = max(dot(normalize(v_normal), L), 0.0);
    vec3 color = vec3(0.96, 0.29, 0.78);
    res.color = clamp(color * 0.5 + color * 0.5 * NL, 0.0, 1.0);
    return res;
}


MaterialReturn deepstone(){
    MaterialReturn res;
    vec3 L = normalize(light_position - v_position);
    float NL = max(dot(normalize(v_normal), L), 0.0);
    vec3 color = vec3(0.29, 0.29, 0.39);
    res.color = clamp(color * 0.4 + color * 0.6 * NL, 0.0, 1.0);
    return res;
}

MaterialReturn stone(){
    MaterialReturn res;
    vec3 L = normalize(light_position - v_position);
    float NL = max(dot(normalize(v_normal), L), 0.0);
    vec3 color = vec3(0.49, 0.39, 0.39);
    res.color = clamp(color * 0.5 + color * 0.5 * NL, 0.0, 1.0);
    return res;
}

MaterialReturn sand(){
    MaterialReturn res;
    vec3 L = normalize(light_position - v_position);
    float NL = max(dot(normalize(v_normal), L), 0.0);
    vec3 color = vec3(0.76, 0.70, 0.50);
    res.color = clamp(color * 0.5 + color * 0.5 * NL, 0.0, 1.0);
    return res;
}

MaterialReturn water(){
    MaterialReturn res;
    vec3 L = normalize(light_position - v_position);
    float NL = max(dot(normalize(v_normal), L), 0.0);
    vec3 color = vec3(0.46, 0.80, 0.84);
    res.color = clamp(color * 0.7 + color * 0.3 * NL, 0.0, 1.0);
    return res;
}

void main() {
    MaterialReturn final;
    switch (material){
        case 0: 
            final = deepstone();
            break;
        case 1:
            final = stone(); 
            break;
        case 2:
            final = sand();
            break;
        case 3:
            final = water();
            break;
        default:
            final = undefined();
            break;
    }

    fragColor = vec4(final.color, 1.0);

}
