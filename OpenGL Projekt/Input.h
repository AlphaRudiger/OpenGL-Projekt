#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class InputProcessor;

extern InputProcessor* currentInputProcessor;

namespace Input {
	void load(GLFWwindow* window);
}

class InputProcessor {
public:
	void virtual touchDown(int button);

};




