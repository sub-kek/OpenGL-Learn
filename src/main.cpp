#include <fstream>
#include <vector>
#include <unordered_map>
#include "engine/graphics.h"
#include "engine/math.h"
#include "engine/input.h"
#include "game.h"

int main() {
    if (!TT::Window::create(1280, 720, "SUPER 3D YOPTA!", true, true)) {
        std::cerr << "Could not create a window...\n";
        return 1;
    }

    Game::World::initialize("obby", 25.0f, glm::vec3(-1.0f, 1.0f, -0.175f));
    Game::Camera::initialize(0.05f, 12.0f, 90.0f);
    Game::Renderer::initialize(TT::Window::getSize());

    Game::Player player(glm::vec3(0.2f, 5.0f, 0.2f), glm::vec3(), glm::vec3(0.4f, 1.6f, 0.4f));

    float fpsUpdateTime = 0.0f;
    int fps = 0;

    TT::Mouse::initialize();
    TT::Mouse::setGrabbed(true);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int frame = 0;
    int mouseGrabFrame = 0;

    glm::vec2 lastWindowSize = TT::Window::getSize();

    TT::Time time;
    while (TT::Window::isRunning()) {
        TT::Window::update();
        TT::Mouse::update();

        Game::Renderer::render(time, player);

        frame++;
        if (TT::Keyboard::isPressed(GLFW_KEY_ESCAPE)) {
            if (!mouseGrabFrame) {
                TT::Mouse::setGrabbed(!TT::Mouse::isGrabbed());
                mouseGrabFrame = frame;
            }
        } else mouseGrabFrame = 0;

        if (player.position.y <= -50.0f) player.respawn();

        glm::vec3 lastPlayerPos = glm::vec3(player.position);
        glm::vec3 lastPlayerAngle = glm::vec3(player.rotation);

        if(TT::Mouse::isGrabbed()) player.update(time);

        time.update();

        fpsUpdateTime += time.getDelta();
        fps++;
        
        if (fpsUpdateTime >= 1.0f) {
            TT::Window::setTitle(std::string("3D Game! Fps: " + std::to_string(fps)).c_str());

            fpsUpdateTime = 0.0f;
            fps = 0;
        }

        glm::vec2 windowSize = TT::Window::getSize();
        if (lastWindowSize != windowSize) {
            lastWindowSize = glm::vec2(windowSize);

            Game::Renderer::resize(windowSize);
        }
    }

    Game::World::clear();
    Game::Renderer::clear();

    player.clear();

    TT::Window::close();

    return 0;
}