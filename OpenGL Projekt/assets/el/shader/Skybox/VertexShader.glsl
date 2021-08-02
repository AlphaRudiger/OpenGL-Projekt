#version 330 core
layout (location = 0) in vec3 position;

out vec3 TexCoords;

uniform mat4 matrix;

void main() {
    TexCoords = position;
    vec4 pos = matrix * vec4(position, 1);
    gl_Position = pos.xyww;
}