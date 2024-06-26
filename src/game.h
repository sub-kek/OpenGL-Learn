#pragma once
#include "engine/graphics.h"
#include "engine/input.h"
#include "engine/math.h"
#include <string>

namespace Game {
    struct Material {
        const glm::vec3 color;

        const float diffuse;
        const float glass;
        const float glassReflect;

        const glm::vec4 uvInfo;

        const bool emissive;

        Material(glm::vec3 color, float diffuse, float glass, float glassReflect, glm::vec4 uvInfo, bool emissive);
    };

    struct Box {
        glm::vec3 position;
        glm::vec3 scale;

        int material;
        std::string tag;

        Box(glm::vec3 position, glm::vec3 scale, int material, std::string tag);
    };
    struct Sphere {
        glm::vec3 position;
        float radius;

        int material;
        std::string tag;

        Sphere(glm::vec3 position, float radius, int material, std::string tag);
    };

    struct Map {
        std::vector<Material> materials;
        std::vector<Box> boxes;
        std::vector<Sphere> spheres;

        int albedoTexture, normalTexture, skyboxTexture;

        Map(
            int albedoTexture, int normalTexture, int skyboxTexture,
            std::vector<Material> materials, std::vector<Box> boxes, std::vector<Sphere> spheres
        );
    };
    class MapParser {
    public:
        static Map parse(const char* location);
    private:
        enum ReadMode {
            INFO, MATERIAL, BOX, SPHERE
        };

        template<typename T> static T parseString(std::string string);

        static std::string getNextSplit(std::stringstream& line, char splitter);
        static std::stringstream getNextStreamSplit(std::stringstream& line, char splitter);

        template<typename T> static T getNextSplit(std::stringstream& line, char splitter);
    };
    struct World {
        static float gravity;
        static float* sunDirection;

        static Map* map;

        static void initialize(const char* mapName, float gravity, glm::vec3 sunDirection);
        static void clear();
    };

    class Player {
    public:
        float walkSpeed, rotateSpeed, jumpHeight, eyeHeight, cinematicSharpness;
        glm::vec3 position, rawRotation, rotation, scale, velocity;

        bool flyMode, cinematicMode;

        Player(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

        void respawn();
        void update(TT::Time time);
        void clear();

        glm::vec3 getEyePosition();
    private:
        const static float onGroundResetDelay, stepSpeed;

        bool rawOnGround = false, onGround = false;
        float onGroundResetDelayTimer, stepTimer;

        glm::vec3 startPosition;

        std::string checkCollision(std::vector<Box> boxes, std::vector<Sphere> spheres) const;
    };

    struct Camera {
        static float dofBlurSize, dofFocusDistance, fov;
        static void initialize(float dofBlurSize, float dofFocusDistance, float fov);
    };

    class Renderer {
    public:
        static void initialize(glm::uvec2 size);
        static void resize(glm::uvec2 size);
        static void reloadShaders();

        static void render(TT::Time time, Player player);
        static void clear();
        static void clearShaders();
        static void clearFrameBuffers();

        static TT::FrameBuffer* getFirstFrameBuffer();
        static TT::FrameBuffer* getSecondFrameBuffer();
    private:
        static TT::ShaderProgram *renderProgram, *screenProgram;
        static TT::FrameBuffer *firstFrameBuffer, *secondFrameBuffer;
    };
}