#ifndef CURVES_H
#define CURVES_H

#include "VAO.h"
#include "VBO.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>


class Shape {
private:
	std::vector<float> verticies;
	GLenum type;
public:
	// Get Fuctions
	std::vector<float> getVerticies();
	GLenum getType();

	// Set Functions
	void addVerticies(float x, float y, float z);
	void setType(GLenum type);
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
	void addShapeToVAO(GLenum type);
	void addShape(GLenum type);
	void addVerticieToShape(float x, float y, float z);
	void deleteVAOs();

	// OpenGL functions
	void processInput(GLFWwindow* window, int height, int width);
	void drawCartesianPlane();
	void drawFromVAO();
	void drawBezierCurve();
};

#endif