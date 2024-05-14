#include "engine/graphics.h"

float color[3] = {0,0,0};

void imgui() {
    ImGui::Begin("Test");

    ImGui::ColorPicker4("Color picker", color, 0);

    ImGui::End();
}

int main() {
    if (!TT::Window::create(1280, 720, "ImGui Impl!", false, 1)) {
        std::cerr << "Could not create a window...\n";
        return 1;
    }

    TT::Window::initializeImGui(TT_IMGUI_THEME_DARK);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (TT::Window::isRunning()) {
        TT::Window::update();

        TT::Window::beginImGui();

        imgui();

        TT::Window::endImGui();
    }

    TT::Window::clearImGui();
    TT::Window::close();

    return 0;
}