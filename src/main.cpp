#include <iostream>
#include "engine/graphics.h"

int main() {
    Graphics::Window::create(1280, 720, "3D", false, false);

    while (Graphics::Window::isRunning()) {
        Graphics::Window::update();
    }
}