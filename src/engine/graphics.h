#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include <streambuf>

#include "../stb/stb_image.h"

namespace TT {
	class Window {
	public:
		static bool create(int width, int height, const char* title, bool resizable, bool vsync);
		static void update();
		static void close();

		static void setTitle(const char* title);
		
		static bool isRunning();

		static glm::vec2 getSize();
		static GLFWwindow *getWindow();
	private:
		static GLFWwindow *window;
	};
	class Shader {
	public:
		Shader(const char* location, GLenum type);
		
		void clear() const;
		int getId() const;
	private:
		int id;
	};
	class ShaderProgram {
	public:
		ShaderProgram();
		
		void addShader(Shader shader);
		bool compile() const;

		void load() const;
		static void unload();

		void clear() const;

		void setUniform(const char* id, int value) const;
		void setUniform(const char* id, float value) const;
		void setUniform(const char* id, glm::vec2 value) const;
		void setUniform(const char* id, glm::vec3 value) const;
		void setUniform(const char* id, glm::vec4 value) const;
	private:
		int id;
		std::vector<Shader> shaders;
	};
	class FrameBuffer {
	public:
		FrameBuffer(int width, int height);

		void load() const;
		void clear() const;

		static void unload();

		int getTexture() const;
		int getWidth() const;
		int getHeight() const;
	private:
		GLuint fboId, rboId, textureId;
		int width, height;
	};
	class Texture {
	public:
		static int loadFromFile(const char* location, GLint filter);
		
		static void load(GLuint texture, int id);
		static void unload();
		static void clear(GLuint texture);
	};
}