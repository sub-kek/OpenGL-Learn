#include <GLFW/glfw3.h>
#include "linmath.h"
#include <stdlib.h>
#include <stdio.h>

#define DEG2RAD 3.1415 / 180

int main() {
    GLFWwindow *window;

    if(!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(1280, 720, "GLFW Window", NULL, NULL);

    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window)) {
        // VIEW SETUP

        float ratio;
        int width, height;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        // DRAWING

        glBegin(GL_LINE_STRIP);
        
        float radius = 0.5;

        glColor3d(1, 0, 0);

        for (int i = 0; i <= 360; i++) {
            float degInRad = i * DEG2RAD;
            glVertex2f(cos(degInRad)*radius, sin(degInRad)*radius*ratio);
        }

        glEnd();

        // SWAP BUFFERS AND CHECK FOR EVENTS

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}