#include <iostream>
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Curves.h"
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {
    // Initial settings
    glfwInit();

    // Version 3.3 of OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "AV1 - Curvas", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
    glewInit();

    // Specify the viewport of OpenGL in the Window
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Controls the State
    State appState;

    glPointSize(10);
    appState.addVerticieToShape(-0.1f, 0.5f, 0.0f);
    appState.addVerticieToShape(0.2f, 0.7f, 0.0f);
    appState.addVerticieToShape(0.5f, 0.5f, 0.0f);
    appState.addVerticieToShape(0.7f, 0.9f, 0.0f);
    
    appState.addShapeToVAO(GL_POINTS);

    appState.drawBezierCurve(0);

    // Generates Shader object using shaders defualt.vert and default.frag
    Shader shaderProgram("default.vert", "default.frag");
    
    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Keyboard and Mouse Input
        appState.processInput(window, SCR_HEIGHT, SCR_WIDTH);

        // Color of the background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        appState.drawCartesianPlane();

        // Tell OpenGL which Shader Program we want to use
        //shaderProgram.Activate();
        // Draw the shapes stored in the VAO
        appState.drawFromVAO();
        
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