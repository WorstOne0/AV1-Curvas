#ifndef CURVES_H
#define CURVES_H

#include <GLFW/glfw3.h>
#include<vector>

class Curve {
private:
	std::vector<float> verticies;
public:
	// Get Fuctions
	std::vector<float> getVerticies();

	// Set Functions
	void addVerticies(float x, float y, float z);
};

class State {
private:
	std::vector<Curve> Curves;
public:
	// Get Functions
	std::vector<Curve> getForms();

	// Set Function
	void addCurve(Curve newCurve);

	// OpenGL functions
	void processInput(GLFWwindow* window);
	void drawCartesianPlane();
};

#endif