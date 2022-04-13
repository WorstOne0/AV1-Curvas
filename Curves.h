#ifndef CURVES_H
#define CURVES_H

#include<vector>

class Form {
private:
	std::vector<float> verticies;
public:
	// Get Fuctions
	std::vector<float> getVerticies();

	// Set Functions
	void addVerticies(float x, float y, float z);
};

class Program {
private:
	std::vector<Form> Forms;
public:
	// Get Functions
	std::vector<Form> getForms();

	// Set Function
	void addform(Form newForm);

	// OpenGL functiona
	void processInput();
};

#endif