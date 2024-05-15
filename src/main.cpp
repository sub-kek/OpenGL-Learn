#include "engine/graphics.h"

int main() {
    if (!TT::Window::create(1280, 720, "OpenGL Engine!", false, 1)) {
        std::cerr << "Could not create a window...\n";
        return 1;
    }

    TT::ShaderProgram *shaderProgram = new TT::ShaderProgram();
    shaderProgram->addShader(TT::Shader("res/shaders/main.vs", GL_VERTEX_SHADER));
    shaderProgram->addShader(TT::Shader("res/shaders/main.fs", GL_FRAGMENT_SHADER));
    shaderProgram->compile();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (TT::Window::isRunning()) {
        TT::Window::update();

        shaderProgram->load();

        glBegin(GL_QUADS);

        glVertex2i(-1, -1);
        glVertex2i(1, -1);
        glVertex2i(1, 1);
        glVertex2i(-1, 1);

        glEnd();

        shaderProgram->unload();
    }

    TT::Window::close();

    return 0;
}