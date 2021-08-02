#include "Input.h"

InputProcessor* currentInputProcessor = nullptr;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (currentInputProcessor == nullptr) return;
	if (action == GLFW_PRESS) {
		currentInputProcessor->touchDown(button);
	}
}


void Input::load(GLFWwindow* window) {
	glfwSetMouseButtonCallback(window, mouse_button_callback);
}

void InputProcessor::touchDown(int button) {

}