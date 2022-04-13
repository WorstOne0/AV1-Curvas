#include "Curves.h"

// Form Class
std::vector<float> Form::getVerticies() {
	return this->verticies;
};

void Form::addVerticies(float x, float y, float z) {
	this->verticies.push_back(x);
	this->verticies.push_back(y);
	this->verticies.push_back(z);
};


// Program Class
std::vector<Form> Program::getForms() {
	return this->Forms;
};
