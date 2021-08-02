#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;

uniform mat4 matrix;

out vec4 o_color;
out vec2 o_texCoord;

void main() {
	gl_Position = matrix * vec4(position, 0, 1);
	o_texCoord = texCoord;
	o_color = color;

}