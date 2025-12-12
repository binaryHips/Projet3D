#version 430

layout(location = 0) in vec3 position;

uniform mat4 MVP;
uniform float pointSize;

out vec3 globalPointPosition;
void main() {
    globalPointPosition = position;
    gl_Position = MVP * vec4(position, 1.0);
    gl_PointSize = pointSize;
    // gl_PointSize = pointSize / gl_Position.w; 
}
