#version 330 core

layout(location = 0) out vec4 color;

in vec4 o_color;
in vec2 o_texCoord;

uniform sampler2D u_texture;

void main() {
	vec4 texColor = texture(u_texture, o_texCoord);
	color = o_color * texColor;
}