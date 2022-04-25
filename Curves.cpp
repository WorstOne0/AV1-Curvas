#include "Curves.h"

float computeBinominal(int n, int k);

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
std::vector<Shape> State::getPoints() {
	return this->Points;
};

std::vector<Shape> State::getCurves() {
	return this->Curves;
};

void State::addPointToVAO(GLenum type) {
	this->newShape.setType(type);

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(this->newShape.getVerticies().data(), this->newShape.getVerticies().size() * sizeof(float));

	// Links VBO to VAO
	VAO1.LinkVBO(VBO1, 0);

	// Adds the VAO to the list
	this->pointVAO.push_back(VAO1);

	// Unbind all to prevent accidentally modifying them
	VBO1.Unbind();
	VAO1.Unbind();
	VBO1.Delete();

	this->Points.push_back(this->newShape);
	// Resets the new shape
	this->newShape.clearData();
};

void State::addCurveToVAO(GLenum type) {
	this->newShape.setType(type);

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(this->newShape.getVerticies().data(), this->newShape.getVerticies().size() * sizeof(float));

	// Links VBO to VAO
	VAO1.LinkVBO(VBO1, 0);

	// Adds the VAO to the list
	this->curveVAO.push_back(VAO1);

	// Unbind all to prevent accidentally modifying them
	VBO1.Unbind();
	VAO1.Unbind();
	VBO1.Delete();

	this->Curves.push_back(this->newShape);
	// Resets the new shape
	this->newShape.clearData();
};

void State::addVerticieToShape(float x, float y, float z) {
	this->newShape.addVerticies(x, y, z);
};

void State::deleteVAOs() {
	for (int i = 0; i < this->pointVAO.size(); i++) {
		this->pointVAO[i].Delete();
	}

	for (int i = 0; i < this->curveVAO.size(); i++) {
		this->curveVAO[i].Delete();
	}
};

// Keyboard and Mouse Input

// Configure the window to be possible to use class functions as callbacks
void State::configureWindow(GLFWwindow* window) {
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	glfwSetWindowUserPointer(window, this);
}

void State::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	State* ptrState = static_cast<State*>(glfwGetWindowUserPointer(window));

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		std::string frase = ptrState->isMouse ? "Teclado" : "Mouse";

		ptrState->isMouse = !ptrState->isMouse;
		std::cout << "O Modo de input agora e pelo " << frase << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		// No points
		if (ptrState->newShape.getVerticies().size() == 0) return;

		ptrState->addPointToVAO(GL_POINTS);
		ptrState->computeBezierCurve(ptrState->Curves.size());
	}
		
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void State::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	State* ptrState = static_cast<State*>(glfwGetWindowUserPointer(window));

	if (ptrState->isMouse) {
		if (ptrState->Curves.size() == 9) {
			std::cout << "Limite maximo de curvas atingido" << std::endl;
			return;
		}

		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			int width, height;
			double xpos = 0, ypos = 0;
			glfwGetCursorPos(window, &xpos, &ypos);
			glfwGetWindowSize(window, &width, &height);

			float x = -1.0f + 2 * xpos / width;
			float y = +1.0f - 2 * ypos / height;

			std::cout << "Ponto adiconado em (" << x << ", " << y << ")" << std::endl;
			ptrState->addVerticieToShape(x, y, 0.0f);
		}
	}
}
	
void State::drawCartesianPlane() {
	// Verticies
	std::vector<float> vertices = {
	1.0f, 0.0f, 0.0f, // Positive X-axis
	-1.0f, 0.0f, 0.0f, // Negative X-axis
	0.0f, 1.0f, 0.0f, // Positive Y-axis
	0.0f, -1.0f, 0.0f, // Negative Y-axis
	};

	for (float i = 0.1f; i < 1.0f; i += 0.1f) {
		// Positive X-axis point 1
		vertices.push_back(i);
		vertices.push_back(0.01f);
		vertices.push_back(0.0f);
		// Positive X-axis point 2
		vertices.push_back(i);
		vertices.push_back(-0.01f);
		vertices.push_back(0.0f);

		// Negative X-axis point 1
		vertices.push_back(-i);
		vertices.push_back(0.01f);
		vertices.push_back(0.0f);
		// Negative X-axis point 2
		vertices.push_back(-i);
		vertices.push_back(-0.01f);
		vertices.push_back(0.0f);

		// Positive Y-axis point 1
		vertices.push_back(0.01f);
		vertices.push_back(i);
		vertices.push_back(0.0f);
		// Positive Y-axis point 2
		vertices.push_back(-0.01f);
		vertices.push_back(i);
		vertices.push_back(0.0f);

		// Negative Y-axis point 1
		vertices.push_back(0.01f);
		vertices.push_back(-i);
		vertices.push_back(0.0f);
		// Negative Y-axis point 2
		vertices.push_back(-0.01f);
		vertices.push_back(-i);
		vertices.push_back(0.0f);
	}

	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices.data(), vertices.size() * sizeof(float));

	// Links VBO to VAO
	VAO1.LinkVBO(VBO1, 0);

	glDrawArrays(GL_LINES, 0, vertices.size());

	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	VBO1.Delete();
}

void State::drawFromPointVAO() {
	for (int i = 0; i < this->pointVAO.size(); i++) {
		this->pointVAO[i].Bind();

		glDrawArrays(this->Points[i].getType(), 0, this->Points[i].getVerticies().size() / 3);
		//glDrawArrays(GL_LINE_STRIP, 0, this->Points[i].getVerticies().size() / 3);

		this->pointVAO[i].Unbind();
	}
}

void State::drawFromCurveVAO() {
	for (int i = 0; i < this->curveVAO.size(); i++) {
		this->curveVAO[i].Bind();

		glDrawArrays(this->Curves[i].getType(), 0, this->Curves[i].getVerticies().size() / 3);

		this->curveVAO[i].Unbind();
	}
}

void State::computeBezierCurve(int index) {
	std::vector<float> bezierCurve;
	
	int n = (this->Points[index].getVerticies().size() / 3) - 1;

	for (float t = 0.0f; t < 1.0f; t += 0.01f) {
		float xPoint = 0.0f, yPoint = 0.0f;

		for (int i = 0; i <= n; i++) {
			xPoint += computeBinominal(n, i) * std::pow((1.0f - t), ((float)n - i)) * std::pow(t, i) * this->Points[index].getVerticies()[3 * i];
			yPoint += computeBinominal(n, i) * std::pow((1.0f - t), ((float)n - i)) * std::pow(t, i) * this->Points[index].getVerticies()[(3 * i) + 1];
			//std::cout << " t= " << t << " i=" << i << " bCurveXt=" << xPoint << " = " << computeBinominal(n, i) << " * " << std::pow((1 - t), (n - i)) << " * " << std::pow(t, i) << " * " << this->Points[index].getVerticies()[3 * i] << std::endl;
			//std::cout << " t= " << t << " i=" << i << " bCurveYt=" << yPoint << " = " << computeBinominal(n, i) << " * " << std::pow((1 - t), (n - i)) << " * " << std::pow(t, i) << " * " << this->Points[index].getVerticies()[3 * i] << std::endl;
		}

		//std::cout << xPoint << " " << yPoint << std::endl;

		bezierCurve.push_back(xPoint);
		bezierCurve.push_back(yPoint);
		bezierCurve.push_back(0.0f);
	}

	for (int i = 0; i < bezierCurve.size() / 3; i++) {
		this->addVerticieToShape(bezierCurve[3 * i], bezierCurve[(3 * i) + 1], bezierCurve[(3 * i) + 2]);
	}

	this->addCurveToVAO(GL_LINE_STRIP);

	bezierCurve.clear();
}

float computeBinominal(int n, int k) {

	float value = 1.0f;

	for (int i = 1; i <= k; i++) {

		value = value * ((n + 1 - i) / i);
	}

	if (n == k) {
		value = 1;
	}

	return value;
}