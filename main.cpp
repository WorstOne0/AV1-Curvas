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

    appState.configureWindow(window);
    
    // Initialize GLEW
    glewInit();

    // Specify the viewport of OpenGL in the Window
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Size of the points
    glPointSize(8);

    // Generates Shader object using shaders defualt.vert and default.frag
    Shader shaderProgram("default.vert", "default.frag");
    
    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Color of the background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);  

        appState.drawCartesianPlane();

        // Tell OpenGL which Shader Program we want to use
        shaderProgram.Activate();
        // Draw the shapes stored in the VAO
        if(appState.drawPoints) appState.drawFromPointVAO();
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

/*void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
        std::cout << "Enter" << std::endl;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        int width, height;
        double xpos = 0, ypos = 0;
        glfwGetCursorPos(window, &xpos, &ypos);
        glfwGetWindowSize(window, &width, &height);

        float x = -1.0f + 2 * xpos / width;
        float y = +1.0f - 2 * ypos / height;
    }
}*/

// Resize window
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // Specify the viewport of OpenGL in the Window
    glViewport(0, 0, width, height);
}