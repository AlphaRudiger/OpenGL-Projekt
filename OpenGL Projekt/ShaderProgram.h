#pragma once

#include <string>

#include <glm.hpp>
#include <unordered_map>
#include <gtc/matrix_transform.hpp>

class ShaderProgram {
	std::unordered_map<std::string, int> map;
	unsigned int program;

public:
	ShaderProgram(std::string vertexShader, std::string fragmentShader, bool isFile);
	void bind();
	int getLocation(std::string& name);
	void setUniform1i(std::string name, int value);
	void setUniformVec2f(std::string name, float x, float y);
	void setUniformVec3f(std::string name, float x, float y, float z);
	void setUniformMat4f(std::string name, glm::mat4 mat);
	~ShaderProgram();

};