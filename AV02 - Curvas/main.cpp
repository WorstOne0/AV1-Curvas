#include <iostream>
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Curves.h"
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
    // Initial settings
    glfwInit();

    // Version 3.3 of OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Controls the State
    State appState;

    // Window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "AV1 - Curvas", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Makes possible for the callbacks functions be inside the class
    appState.configureWindow(window);
    
    // Initialize GLEW
    glewInit();

    // Specify the viewport of OpenGL in the Window
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Size of the points
    glPointSize(8);

    // Generates Shader object using shaders defualt.vert and default.frag
    Shader shaderProgram("default.vert", "default.frag");

    std::cout << "O modo de input padrao e pelo mouse, cada clique na tela gera um ponto" << std::endl;
    std::cout << "A tecla enter insere uma nova curva" << "\n\n";

    std::cout << "A tecla P adiciona/remove os pontos" << std::endl;
    std::cout << "A tecla S liga/desliga o input pelo mouse/teclado" << std::endl;
    std::cout << "Quando o input e pelo teclado a tecla I inicia a digitacao pelo console" << std::endl;
    
    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Color of the background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Tell OpenGL which Shader Program we want to use
        shaderProgram.Activate();

        // Draw the cartesian plan
        appState.drawCartesianPlane();

        // Get the input from console when
        if (appState.isConsole) appState.getFromConsole();

        // Draw the points stored in the VAO
        if (appState.drawPoints) appState.drawFromPointVAO();
        // Draw the curves stored in the VAO
        appState.drawFromCurveVAO();
        
        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Delete all the objects
    shaderProgram.Delete();
    appState.deleteVAOs();

    // Delete window before ending
    glfwDestroyWindow(window);
    // Clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// Resize window
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // Specify the viewport of OpenGL in the Window
    glViewport(0, 0, width, height);
}