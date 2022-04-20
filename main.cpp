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
    appState.addVerticieToShape(0.0f, 0.0f, 0.0f);
    appState.addVerticieToShape(0.4f, 0.8f, 0.0f);
    appState.addVerticieToShape(0.8f, 0.4f, 0.0f);
    appState.addVerticieToShape(1.0f, 1.0f, 0.0f);

    appState.addShapeToVAO(GL_POINTS);
   
    /*
    // Draw a Quad with 2 triangles without indicies
    // 1st Triangle 
    appState.addVerticieToShape(-0.5f, -0.5f, 0.0f);
    appState.addVerticieToShape(0.5f, -0.5f, 0.0f);
    appState.addVerticieToShape(-0.5f, 0.5f, 0.0f);
    // 2nd Trinagle
    appState.addVerticieToShape(0.5f, 0.5f, 0.0f);
    appState.addVerticieToShape(0.5f, -0.5f, 0.0f);
    appState.addVerticieToShape(-0.5f, 0.5f, 0.0f);

    // Add the shape to the VAO
    appState.addShapeToVAO(GL_TRIANGLES);

    appState.addVerticieToShape(0.0f, 1.0f, 0.0f);
    appState.addVerticieToShape(-1.0f, 0.0f, 0.0f);

    // Add the shape to the VAO
    appState.addShapeToVAO(GL_LINES);

    glPointSize(40);
    appState.addVerticieToShape(0.0f, 0.5f, 0.0f);

    // Add the shape to the VAO
    appState.addShapeToVAO(GL_POINTS);

    // Add each position x, y, z to the shape to be drawn
    appState.addVerticieToShape(-1.0f, -1.0f, 0.0f);
    appState.addVerticieToShape(0.0f, -1.0f, 0.0f);
    appState.addVerticieToShape(-1.0f, 0.0f, 0.0f);

    // Add the shape to the VAO
    appState.addShapeToVAO(GL_TRIANGLES);*/

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