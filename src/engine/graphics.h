#pragma once
#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

namespace Graphics {
    class Window {
	    public:
		    static bool create(int width, int height, const char* title, bool resizable, bool verticalSync);
		    static void update();
		    static void close();

		    static void setTitle(const char* title);
    
		    static bool isRunning();

		    static glm::vec2 getSize();
		    static GLFWwindow* getId();
	    private:
		    static GLFWwindow* window;
	};
}