#include "graphics.h"

GLFWwindow* TT::Window::window = NULL;

bool TT::Window::create(int width, int height, const char* title, bool resizable, bool vsync) {
	if (!glfwInit()) {
		std::cerr << "GLFW has't been Initialized!\n";
		return false;
	}

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window) {
		glfwTerminate();
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
	
	GLenum error = glewInit();
	if (error != GLEW_OK && error != GLEW_ERROR_NO_GLX_DISPLAY) {
		std::cerr << "Glew init error: " << glewGetErrorString(error) << "\n";
		return false;
	}

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


TT::Shader::Shader(const char* location, GLenum type) {
	std::ifstream stream(location);
	std::string code{ std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>() };

	const char* charCode = code.c_str();

	id = glCreateShader(type);
	glShaderSource(id, 1, &charCode, NULL);
	glCompileShader(id);

	int compiled = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {
		char log[512];

		glGetShaderInfoLog(id, 512, NULL, log);
		std::cerr << "Error in shader:\n" << log << '\n';
	}
}

void TT::Shader::clear() const {
	glDeleteShader(id);
}
int TT::Shader::getId() const {
	return id;
}

TT::ShaderProgram::ShaderProgram() {
	id = glCreateProgram();
}

void TT::ShaderProgram::addShader(Shader shader) {
	shaders.push_back(shader);
	glAttachShader(id, shader.getId());
}
bool TT::ShaderProgram::compile() const {
	bool success = true;

	glLinkProgram(id);

	int linked = 0;
	glGetProgramiv(id, GL_LINK_STATUS, &linked);
	
	if (!linked) {
		char log[512];
		glGetProgramInfoLog(id, 512, NULL, log);

		std::cerr << "Shader not linked\nError:\n" << log << '\n';
		success = false;
	}


	glValidateProgram(id);

	int compiled = 0;
	glGetProgramiv(id, GL_VALIDATE_STATUS, &compiled);

	if (!compiled) {
		char log[512];
		glGetProgramInfoLog(id, 512, NULL, log);

		std::cerr << "Shader not compiled!\nError:\n" << log << '\n';
		success = false;
	}

	return success;
}

void TT::ShaderProgram::load() const {
	glUseProgram(id);
}
void TT::ShaderProgram::unload() {
	glUseProgram(0);
}
void TT::ShaderProgram::clear() const {
	for (auto& shader : shaders) {
		glDetachShader(id, shader.getId());
		shader.clear();
	}

	glDeleteProgram(id);
}

void TT::ShaderProgram::setUniform(const char* id, int value) const {
	glUniform1i(glGetUniformLocation(this->id, id), value);
}
void TT::ShaderProgram::setUniform(const char* id, float value) const {
	glUniform1f(glGetUniformLocation(this->id, id), value);
}
void TT::ShaderProgram::setUniform(const char* id, glm::vec2 value) const {
	glUniform2f(glGetUniformLocation(this->id, id), value.x, value.y);
}
void TT::ShaderProgram::setUniform(const char* id, glm::vec3 value) const {
	glUniform3f(glGetUniformLocation(this->id, id), value.x, value.y, value.z);
}
void TT::ShaderProgram::setUniform(const char* id, glm::vec4 value) const {
	glUniform4f(glGetUniformLocation(this->id, id), value.x, value.y, value.z, value.w);
}