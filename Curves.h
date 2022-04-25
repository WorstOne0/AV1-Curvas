#ifndef CURVES_H
#define CURVES_H

#include "VAO.h"
#include "VBO.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

float colors[] = { 
	0.0f, 0.0f, 0.0f, // Black
	1.0f, 1.0f, 1.0f, // White
};

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
	std::vector<VAO> pointVAO, curveVAO;
	std::vector<Shape> Points, Curves;
	Shape newShape;

public:
	bool drawPoints = true, isMouse = true;

	// Get Functions
	std::vector<Shape> getPoints();
	std::vector<Shape> getCurves();

	// Set Function
	void addPointToVAO(GLenum type);
	void addCurveToVAO(GLenum type);
	void addVerticieToShape(float x, float y, float z);
	void deleteVAOs();

	// OpenGL functions
	void configureWindow(GLFWwindow* window);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void drawCartesianPlane();
	void drawFromPointVAO();
	void drawFromCurveVAO();
	void computeBezierCurve(int index);
};

#endif