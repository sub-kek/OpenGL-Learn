#include "graphics.h"

GLFWwindow* TT::Window::window = nullptr;

bool TT::Window::create(int width, int height, const char* title, bool resizable, bool vsync) {
	if (!glfwInit()) {
		std::cerr << "GLFW has't been Initialized!\n";
		return false;
	}

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window) {
		std::cerr << "Window not created caused by something!\n";
		return false;
	}

	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, [](GLFWwindow* _window, int _width, int _height) {
		glViewport(0, 0, _width, _height);
	});

	const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	if (videoMode != NULL)
		glfwSetWindowPos(window, (videoMode->width - width) / 2, (videoMode->height - height) / 2);

	glfwSwapInterval(vsync);
	glfwShowWindow(window);

	return true;
}
void TT::Window::update() {
	glfwSwapBuffers(window);
	glfwPollEvents();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void TT::Window::close() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void TT::Window::setTitle(const char* title) {
	glfwSetWindowTitle(window, title);
}

void TT::Window::initializeImGui(unsigned int theme) {
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 120");
}
void TT::Window::beginImGui() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::NewFrame();
}
void TT::Window::endImGui() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void TT::Window::clearImGui() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	ImGui::DestroyContext();
}

bool TT::Window::isRunning() {
	return !glfwWindowShouldClose(window);
}

glm::vec2 TT::Window::getSize() {
	int width = 0, height = 0;
	glfwGetWindowSize(window, &width, &height);

	return glm::vec2(width, height);
}

GLFWwindow* TT::Window::getId() {
	return window;
}