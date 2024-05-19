#include "input.h"

glm::vec2 TT::Mouse::velocity;
glm::vec2 TT::Mouse::lastPosition;

bool TT::Keyboard::isPressed(int key) {
	return glfwGetKey(TT::Window::getWindow(), key) == GLFW_PRESS;
}

void TT::Mouse::initialize() {
	lastPosition = getPosition();
}
void TT::Mouse::update() {
	glm::vec2 position = getPosition();
	velocity = position - lastPosition;
	
	lastPosition = position;
}

void TT::Mouse::setGrabbed(bool grabbed) {
	glfwSetInputMode(TT::Window::getWindow(), GLFW_CURSOR, grabbed ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

bool TT::Mouse::isGrabbed() {
	return glfwGetInputMode(TT::Window::getWindow(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
}
bool TT::Mouse::isPressed(int button) {
	return glfwGetMouseButton(TT::Window::getWindow(), button) == GLFW_PRESS;
}

glm::vec2 TT::Mouse::getVelocity() {
	return glm::vec2(velocity);
}
glm::vec2 TT::Mouse::getPosition() {
	double x, y;
	glfwGetCursorPos(TT::Window::getWindow(), &x, &y);

	return glm::vec2((float)x, (float)y);
}