#include "Curves.h"

// Form Class
std::vector<float> Curve::getVerticies() {
	return this->verticies;
};

void Curve::addVerticies(float x, float y, float z) {
	this->verticies.push_back(x);
	this->verticies.push_back(y);
	this->verticies.push_back(z);
};


// Program Class
std::vector<Curve> State::getForms() {
	return this->Curves;
};

void State::addCurve(Curve newCurve) {
	
};

// Keyboard and Mouse Input
void State::processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
};