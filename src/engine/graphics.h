#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#define TT_IMGUI_THEME_DARK 0
#define TT_IMGUI_THEME_LIGHT 1
#define TT_IMGUI_THEME_CLASSIC 2

namespace TT {
	class Window {
	public:
		static bool create(int width, int height, const char* title, bool resizable, bool vsync);
		static void update();
		static void close();

		static void initializeImGui(unsigned int theme);
		static void beginImGui();
		static void endImGui();
		static void clearImGui();

		static void setTitle(const char* title);
		
		static bool isRunning();

		static glm::vec2 getSize();
		static GLFWwindow* getId();
	private:
		static GLFWwindow* window;
	};
}