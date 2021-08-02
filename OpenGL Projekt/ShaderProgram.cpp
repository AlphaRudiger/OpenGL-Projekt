#include "ShaderProgram.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Utils.h"

typedef unsigned int uint;

using std::string;


string gets(string sf) {
	string line;
	std::stringstream ss;
	std::ifstream stream(sf);
	if (stream.fail()) {
		std::cout << "Unable to read shader file: " << sf << std::endl;
	}
	while (std::getline(stream, line)) {
		ss << line << '\n';
	}
	stream.close();
	return ss.str();
}

uint compileShader(uint type, string source) {
	GLCall(uint id = glCreateShader(type));
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = new char[length];
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << message << std::endl;
		delete[] message;
		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

ShaderProgram::ShaderProgram(string vertexShader, string fragmentShader, bool isFile) {
	if (isFile) {
		vertexShader = gets(vertexShader);
		fragmentShader = gets(fragmentShader);
	}

	GLCall(program = glCreateProgram());
	uint vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	uint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));
}

void ShaderProgram::bind() {
	glUseProgram(program);
}

int ShaderProgram::getLocation(string& name) {
	auto s = map.find(name);
	int id;
	if (s == map.end()) {
		GLCall(id = glGetUniformLocation(program, name.c_str()));
		map.insert(std::pair<string, int>(name, id));
	} else {
		id = s->second;
	}

	return id;
}

void ShaderProgram::setUniform1i(std::string name, int value) {
	GLCall(glUniform1i(getLocation(name), value));
}

void ShaderProgram::setUniformVec2f(string name, float x, float y) {
	GLCall(glUniform2f(getLocation(name), x, y));
}

void ShaderProgram::setUniformVec3f(string name, float x, float y, float z) {
	GLCall(glUniform3f(getLocation(name), x, y, z));
}


void ShaderProgram::setUniformMat4f(string name, glm::mat4 mat) {
	GLCall(glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, &mat[0][0]));
}

ShaderProgram::~ShaderProgram() {
	GLCall(glDeleteProgram(program));
}