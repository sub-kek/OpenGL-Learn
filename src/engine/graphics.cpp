#include "graphics.h"

GLFWwindow* Graphics::Window::window = nullptr;

bool Graphics::Window::create(int width, int height, const char* title, bool resizable, bool verticalSync) {
	if (!glfwInit()) {
		std::cerr << "GLFW not initialized!\n";
		return false;
	}

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window) {
		std::cerr << "Window not created!\n";
		return false;
	}

	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, [](GLFWwindow* _window, int _width, int _height) {
		glViewport(0, 0, _width, _height);
	});

	const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	if (videoMode != NULL)
		glfwSetWindowPos(window, (videoMode->width - width) / 2, (videoMode->height - height) / 2);

	glfwSwapInterval(verticalSync);
	glfwShowWindow(window);

	return true;
}
void Graphics::Window::update() {
	glfwSwapBuffers(window);
	glfwPollEvents();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Graphics::Window::close() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Graphics::Window::setTitle(const char* title) {
	glfwSetWindowTitle(window, title);
}

bool Graphics::Window::isRunning() {
	return !glfwWindowShouldClose(window);
}

glm::vec2 Graphics::Window::getSize() {
	int width = 0, height = 0;
	glfwGetWindowSize(window, &width, &height);

	return glm::vec2(width, height);
}

GLFWwindow* Graphics::Window::getId() {
	return window;
}