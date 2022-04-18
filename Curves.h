#ifndef CURVES_H
#define CURVES_H

#include "VAO.h"
#include "VBO.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <string>


class Shape {
private:
	std::vector<float> verticies;
	std::string type;
public:
	// Get Fuctions
	std::vector<float> getVerticies();

	// Set Functions
	void addVerticies(float x, float y, float z);
	void clearData();
};

class State {
private:
	std::vector<VAO> VAOs;
	std::vector<Shape> Shapes;
	Shape newShape;

public:
	// Get Functions
	std::vector<VAO> getVAOs();
	std::vector<Shape> getShapes();

	// Set Function
	void addShapeToVAO();
	void addShape();
	void addVerticieToShape(float x, float y, float z);
	void deleteVAOs();

	// OpenGL functions
	void processInput(GLFWwindow* window);
	void drawCartesianPlane();
	void drawFromVAO();
};

#endif