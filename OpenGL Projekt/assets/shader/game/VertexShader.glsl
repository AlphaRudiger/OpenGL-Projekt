#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec4 color;
layout(location = 3) in vec2 texCoord;

uniform mat4 matrix;
uniform vec3 playerPosition;
uniform vec3 lightPos;

out vec4 o_color;
out vec2 o_texCoord;

float ambient = 0.5f;

void main() {
	vec4 test = matrix * vec4(position, 1);
	gl_Position = (matrix * vec4(position, 1)) * 2;
	o_texCoord = texCoord;

	//vec3 lightDir = normalize(vec3(-0.1, 0.4, -0.2));//normalize(lightPos - position);
	//float diffuse = max(dot(normal, lightDir), -10);
	//
	//if(diffuse < 0) {
	//	diffuse /= 5;
	//}
	//
	//float light = min(diffuse + ambient, 1);
	//vec4 color = vec4(light, light, light, 1);

	//vec3 vdist = playerPosition - position;
	//
	//vec2 v = normalize(vec2(vdist.x, vdist.z)) * 100;
	//
	//if(abs(vdist.x + vdist.z) > abs(v.x + v.y)) color = vec4(1, 1, 1, 0);

	o_color = vec4(color.xyz, 1);

}