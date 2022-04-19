#include "Curves.h"

// Form Class
std::vector<float> Shape::getVerticies() {
	return this->verticies;
};

GLenum Shape::getType() {
	return this->type;
}

void Shape::addVerticies(float x, float y, float z) {
	this->verticies.push_back(x);
	this->verticies.push_back(y);
	this->verticies.push_back(z);
};

void Shape::setType(GLenum type) {
	this->type = type;
}

void Shape::clearData() {
	this->verticies.clear();
};


// Program Class
std::vector<VAO> State::getVAOs() {
	return this->VAOs;
};

std::vector<Shape> State::getShapes() {
	return this->Shapes;
};

void State::addShapeToVAO(GLenum type) {
	this->addShape(type);

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(this->newShape.getVerticies().data(), this->newShape.getVerticies().size() * sizeof(float));

	// Links VBO to VAO
	VAO1.LinkVBO(VBO1, 0);

	// Adds the VAO to the list
	this->VAOs.push_back(VAO1);

	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	VBO1.Delete();

	// Resets the new shape
	this->newShape.clearData();
};

void State::addShape(GLenum type) {
	this->newShape.setType(type);
	this->Shapes.push_back(this->newShape);
};

void State::addVerticieToShape(float x, float y, float z) {
	this->newShape.addVerticies(x, y, z);
};

void State::deleteVAOs() {
	for (int i = 0; i < this->VAOs.size(); i++) {
		this->VAOs[i].Delete();
	}
};

// Keyboard and Mouse Input
void State::processInput(GLFWwindow* window) {
	double xpos = 0, ypos = 0;

	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

	if (state == GLFW_PRESS) {
		glfwGetCursorPos(window, &xpos, &ypos);
		std::cout << xpos << " " << ypos << "\n";
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
};
	
void State::drawCartesianPlane() {
	// Verticies
	std::vector<float> vertices = {
	1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	};
}

void State::drawFromVAO() {
	for (int i = 0; i < this->VAOs.size(); i++) {
		this->VAOs[i].Bind();

		// Draw the triangle using the GL_TRIANGLES primitive
		glDrawArrays(this->Shapes[i].getType(), 0, this->Shapes[i].getVerticies().size());

		this->VAOs[i].Unbind();
	}
}