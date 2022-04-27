#include "Curves.h"

float computeBinominal(int n, int k);
double blend(std::vector<float>& uVec, double t, int k, int d);

// Form Class
std::vector<float> Shape::getVerticies() {
	return this->verticies;
};

GLenum Shape::getType() {
	return this->type;
}

void Shape::addVerticies(float x, float y, float z, float r, float g, float b) {
	// Verticies
	this->verticies.push_back(x);
	this->verticies.push_back(y);
	this->verticies.push_back(z);

	// Colors
	this->verticies.push_back(r);
	this->verticies.push_back(g);
	this->verticies.push_back(b);
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
	// Type to be drawn
	this->newShape.setType(type);

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(this->newShape.getVerticies().data(), this->newShape.getVerticies().size() * sizeof(float));

	// Links VBO to VAO
	// Vertex
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	// Color
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	// Adds the VAO to the list
	this->pointVAO.push_back(VAO1);

	// Unbind all to prevent accidentally modifying them
	VBO1.Unbind();
	VAO1.Unbind();
	VBO1.Delete();

	// Adds the shape to the array
	this->Points.push_back(this->newShape);

	// Resets the new shape
	this->newShape.clearData();
};

void State::addCurveToVAO(GLenum type) {
	// Type to be drawn
	this->newShape.setType(type);

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(this->newShape.getVerticies().data(), this->newShape.getVerticies().size() * sizeof(float));

	// Links VBO to VAO
	// Vertex
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	// Color
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	// Adds the VAO to the list
	this->curveVAO.push_back(VAO1);

	// Unbind all to prevent accidentally modifying them
	VBO1.Unbind();
	VAO1.Unbind();
	VBO1.Delete();

	// Adds the shape to the array
	this->Curves.push_back(this->newShape);

	// Resets the new shape
	this->newShape.clearData();
};

void State::addVerticieToShape(float x, float y, float z, float r, float g, float b) {
	this->newShape.addVerticies(x, y, z, r, g, b);
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

	// Swap between mouse and keyboard input
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		std::string frase = ptrState->isMouse ? "Teclado. Tecle I para ativar a digitalizacao no console \n" : "Mouse \n";

		// Swap
		ptrState->isMouse = !ptrState->isMouse;
		std::cout << "O Modo de input agora e pelo " << frase << std::endl;
	}

	if (!ptrState->isMouse) {
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
			ptrState->isConsole = true;

			std::cout << "A tela esta travada enquanto o modo de digitacao pelo console esta ativo, selecione o console" << std::endl;
		}
	}

	// Swap between mouse and keyboard input
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		std::string frase = ptrState->drawPoints ? "desativados" : "ativados";

		// Swap
		ptrState->drawPoints = !ptrState->drawPoints;
		std::cout << "Os pontos foram " << frase << std::endl;
	}

	// Submits the points
	if (!ptrState->isConsole) {
		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
			// No points
			if (ptrState->newShape.getVerticies().size() == 0) return;

			// Min 3 points
			if ((ptrState->newShape.getVerticies().size() / 6) < 3) {
				std::cout << "Essa funcao precisa(na verdade nao precisa mais e o que ta sendo pedido) de 3 pontos";
				return;
			}

			ptrState->addPointToVAO(GL_POINTS);
			ptrState->computeBezierCurve(ptrState->Curves.size());
			// ptrState->computeBSpline(ptrState->Curves.size());
		}
	}
		
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void State::getFromConsole() {
	std::string a;
	std::cout << "Digite o " << (this->newShape.getVerticies().size() / 6) + 1 << " verticie" << std::endl;

	std::cout << "X: ";
	std::cin >> a;
	float x = std::stof(a);

	std::cout << "Y: ";
	std::cin >> a;
	float y = std::stof(a);

	// NDC
	if ((x > 1.0f || y > 1.0f) || (x < -1.0f || y < -1.0f)) {
		std::cout << "O Ponto adiconado em (" << x << ", " << y << ") e invalido e sera descartado" << std::endl;
		return;
	}

	// No repeated points for the same curve
	for (int i = 0; i < this->newShape.getVerticies().size() / 6; i++) {
		if (this->newShape.getVerticies()[(6 * i)] == x && this->newShape.getVerticies()[(6 * i) + 1] == y) {
			std::cout << "O Ponto adiconado em (" << x << ", " << y << ") e repetido e sera descartado" << std::endl;
			return;
		}
	}

	// Add the verticies with a color to the shape
	this->addVerticieToShape(x, y, 0.0f, this->colors[(3 * this->Curves.size())], this->colors[(3 * this->Curves.size()) + 1], this->colors[(3 * this->Curves.size()) + 2]);

	std::cout << "Digite no console 'q' para sair. Digite 'i' pra continuar" << std::endl;
	std::cin >> a;

	// Max 9 points or user finished inserting
	if (a == "q" || (this->newShape.getVerticies().size() / 6) >= 9) {
		this->isConsole = false;

		this->addPointToVAO(GL_POINTS);
		this->computeBezierCurve(this->Curves.size());

		std::cout << "O modo de input pelo console terminou, selecione novamente a tela" << std::endl;
	}
}

void State::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	State* ptrState = static_cast<State*>(glfwGetWindowUserPointer(window));

	if (ptrState->isMouse) {
		// Max of 5 curves
		if (ptrState->Curves.size() == 5) {
			std::cout << "Limite maximo de curvas atingido" << std::endl;
			return;
		}

		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			int width, height;
			double xpos = 0, ypos = 0;

			// Get mouse position
			glfwGetCursorPos(window, &xpos, &ypos);
			// Get window size
			glfwGetWindowSize(window, &width, &height);

			// Convert to NDC coordinates
			float x = -1.0f + 2 * xpos / width;
			float y = +1.0f - 2 * ypos / height;

			// Max 9 points per curve
			if ((ptrState->newShape.getVerticies().size() / 6) >= 9) {
				std::cout << "Numero maximo d verticies atingido, aperte enter para enviar a curva" << std::endl;
				return;
			}

			// No repeated points for the same curve
			for (int i = 0; i < ptrState->newShape.getVerticies().size() / 6; i++) {
				if (ptrState->newShape.getVerticies()[(6 * i)] == x && ptrState->newShape.getVerticies()[(6 * i) + 1] == y) {
					std::cout << "O Ponto adiconado em (" << x << ", " << y << ") e repetido e sera descartado" << std::endl;
					return;
				}
			}

			std::cout << "Ponto adiconado em (" << x << ", " << y << ")" << std::endl;

			// Add the verticies with a color to the shape
			ptrState->addVerticieToShape(x, y, 0.0f, ptrState->colors[(3 * ptrState->Curves.size())], ptrState->colors[(3 * ptrState->Curves.size()) + 1], ptrState->colors[(3 * ptrState->Curves.size()) + 2]);
		}
	}
}
	
void State::drawCartesianPlane() {
	// Verticies
	std::vector<float> vertices = {
	//   POSITION      /       COLOR(Black)
	1.0f, 0.0f, 0.0f,     0.0f, 0.0f, 0.0f, // Positive X-axis
	-1.0f, 0.0f, 0.0f,    0.0f, 0.0f, 0.0f, // Negative X-axis
	0.0f, 1.0f, 0.0f,     0.0f, 0.0f, 0.0f, // Positive Y-axis
	0.0f, -1.0f, 0.0f,    0.0f, 0.0f, 0.0f, // Negative Y-axis
	};

	for (float i = 0.1f; i < 1.0f; i += 0.1f) {
		// Positive X-axis point 1
		vertices.push_back(i);
		vertices.push_back(0.01f);
		vertices.push_back(0.0f);

		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);

		// Positive X-axis point 2
		vertices.push_back(i);
		vertices.push_back(-0.01f);
		vertices.push_back(0.0f);

		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);

		// Negative X-axis point 1
		vertices.push_back(-i);
		vertices.push_back(0.01f);
		vertices.push_back(0.0f);

		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);

		// Negative X-axis point 2
		vertices.push_back(-i);
		vertices.push_back(-0.01f);
		vertices.push_back(0.0f);

		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);

		// Positive Y-axis point 1
		vertices.push_back(0.01f);
		vertices.push_back(i);
		vertices.push_back(0.0f);

		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);

		// Positive Y-axis point 2
		vertices.push_back(-0.01f);
		vertices.push_back(i);
		vertices.push_back(0.0f);

		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);

		// Negative Y-axis point 1
		vertices.push_back(0.01f);
		vertices.push_back(-i);
		vertices.push_back(0.0f);

		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);

		// Negative Y-axis point 2
		vertices.push_back(-0.01f);
		vertices.push_back(-i);
		vertices.push_back(0.0f);

		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
	}

	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices.data(), vertices.size() * sizeof(float));

	// Links VBO to VAO
	// Vertex
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	// Color
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	glDrawArrays(GL_LINES, 0, vertices.size() / 6);

	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	VBO1.Delete();
}

void State::drawFromPointVAO() {
	// For each shape on the VAO
	for (int i = 0; i < this->pointVAO.size(); i++) {
		// Bind the VAO to know wicth points it should to draw
		this->pointVAO[i].Bind();

		glDrawArrays(this->Points[i].getType(), 0, this->Points[i].getVerticies().size() / 6);
		//glDrawArrays(GL_LINE_STRIP, 0, this->Points[i].getVerticies().size() / 3);

		this->pointVAO[i].Unbind();
	}
}

void State::drawFromCurveVAO() {
	// For each shape on the VAO
	for (int i = 0; i < this->curveVAO.size(); i++) {
		// Bind the VAO to know wicth points it should to draw
		this->curveVAO[i].Bind();

		glDrawArrays(this->Curves[i].getType(), 0, this->Curves[i].getVerticies().size() / 6);

		this->curveVAO[i].Unbind();
	}
}

// Generate the points of a bezier curve
void State::computeBezierCurve(int index) {
	std::vector<float> bezierCurve;
	
	int n = (this->Points[index].getVerticies().size() / 6) - 1;

	for (float t = 0.0f; t < 1.0f; t += 0.01f) {
		float xPoint = 0.0f, yPoint = 0.0f;

		for (int i = 0; i <= n; i++) {
			xPoint += computeBinominal(n, i) * std::pow((1.0f - t), ((float)n - i)) * std::pow(t, i) * this->Points[index].getVerticies()[6 * i];
			yPoint += computeBinominal(n, i) * std::pow((1.0f - t), ((float)n - i)) * std::pow(t, i) * this->Points[index].getVerticies()[(6 * i) + 1];
			//std::cout << " t= " << t << " i=" << i << " bCurveXt=" << xPoint << " = " << computeBinominal(n, i) << " * " << std::pow((1 - t), (n - i)) << " * " << std::pow(t, i) << " * " << this->Points[index].getVerticies()[3 * i] << std::endl;
			//std::cout << " t= " << t << " i=" << i << " bCurveYt=" << yPoint << " = " << computeBinominal(n, i) << " * " << std::pow((1 - t), (n - i)) << " * " << std::pow(t, i) << " * " << this->Points[index].getVerticies()[3 * i] << std::endl;
		}

		//std::cout << xPoint << " " << yPoint << std::endl;

		bezierCurve.push_back(xPoint);
		bezierCurve.push_back(yPoint);
		bezierCurve.push_back(0.0f);
	}

	for (int i = 0; i < bezierCurve.size() / 3; i++) {
		// Add the verticies with a color to the shape
		this->addVerticieToShape(bezierCurve[3 * i], bezierCurve[(3 * i) + 1], bezierCurve[(3 * i) + 2], this->colors[(3 * this->Curves.size())], this->colors[(3 * this->Curves.size()) + 1], this->colors[(3 * this->Curves.size()) + 2]);
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

void State::computeBSpline(int index) {
	std::vector<float> curve, uVec;

	int n = (this->Points[index].getVerticies().size() / 6), d = 15;

	for (int i = 0; i < n + d; i++) {
		uVec.push_back(((float)i) / (n + d - 1));
	}

	for (float t = 0.0f; t < 1.0f; t += 0.1f) {
		float xPoint = 0.0f, yPoint = 0.0f;

		for (int i = 0; i < n - 1; i++) {
			xPoint += blend(uVec, t, i, d) * this->Points[index].getVerticies()[6 * i];
			yPoint += blend(uVec, t, i, d) * this->Points[index].getVerticies()[(6 * i) + 1];
		}

		curve.push_back(xPoint);
		curve.push_back(yPoint);
		curve.push_back(0.0f);
	}

	for (int i = 0; i < curve.size() / 3; i++) {
		// Add the verticies with a color to the shape
		this->addVerticieToShape(curve[3 * i], curve[(3 * i) + 1], curve[(3 * i) + 2], this->colors[(3 * this->Curves.size())], this->colors[(3 * this->Curves.size()) + 1], this->colors[(3 * this->Curves.size()) + 2]);
	}

	this->addCurveToVAO(GL_LINE_STRIP);

	curve.clear();
}

double blend(std::vector<float>& uVec, double t, int k, int d) {
	if (d == 1) {
		if (uVec[k] <= t && t < uVec[k + 1])
			return 1;
		return 0;
	}

	double b;

	b = ((t - uVec[k]) / (uVec[k + d - 1] - uVec[k]) * blend(uVec, t, k, d - 1)) + ((uVec[k + d] - t) / (uVec[k + d] - uVec[k + 1]) * blend(uVec, t, k + 1, d - 1));
	
	return b;
}