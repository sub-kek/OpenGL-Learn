#include "game.h"

Game::Material::Material(glm::vec3 color, float diffuse, float glass, float glassReflect, glm::vec4 uvInfo, bool emissive)
    : color(color), diffuse(diffuse), glass(glass), glassReflect(glassReflect), uvInfo(uvInfo), emissive(emissive) {};

Game::Box::Box(glm::vec3 position, glm::vec3 scale, int material, std::string tag) : position(position), scale(scale), material(material), tag(tag) {}
Game::Sphere::Sphere(glm::vec3 position, float radius, int material, std::string tag) : position(position), radius(radius), material(material), tag(tag) {}

Game::Map::Map(
    int albedoTexture, int normalTexture, int skyboxTexture,
    std::vector<Game::Material> materials, std::vector<Game::Box> boxes, std::vector<Game::Sphere> spheres
) :
    albedoTexture(albedoTexture), normalTexture(normalTexture), skyboxTexture(skyboxTexture),
    materials(materials), boxes(boxes), spheres(spheres)
{}

Game::Map Game::MapParser::parse(const char* location) {
    std::ifstream file(location);

    std::vector<Game::Material> materials;
    std::vector<Game::Box> boxes;
    std::vector<Game::Sphere> spheres;

    int albedoTexture = 0, normalTexture = 0, skyboxTexture = 0;

    if (!file.is_open()) {
        throw std::runtime_error(std::string("Could not parse map: \"" + std::string(location) + "\""));
        return Game::Map(albedoTexture, normalTexture, skyboxTexture, materials, boxes, spheres);
    }

    ReadMode readMode = INFO;

    while (file) {
        std::string line;
        getline(file, line);

        if (line.starts_with("Info")) readMode = INFO;
        else if (line.starts_with("Materials")) readMode = MATERIAL;
        else if (line.starts_with("Boxes")) readMode = BOX;
        else if (line.starts_with("Spheres")) readMode = SPHERE;
        else {
            if (line == "" || line.starts_with("//")) continue;

            std::stringstream lineStream(line);

            if (readMode == INFO) {
                albedoTexture = TT::Texture::loadFromFile(("res/textures/" + getNextSplit(lineStream, '/')).c_str(), GL_LINEAR);
                normalTexture = TT::Texture::loadFromFile(("res/textures/" + getNextSplit(lineStream, '/')).c_str(), GL_LINEAR);
                skyboxTexture = TT::Texture::loadFromFile(("res/textures/" + getNextSplit(lineStream, '/')).c_str(), GL_LINEAR);
            }
            else if (readMode == MATERIAL) {
                std::stringstream vectorStream = getNextStreamSplit(lineStream, '/');

                float red = getNextSplit<float>(vectorStream, ',');
                float green = getNextSplit<float>(vectorStream, ',');
                float blue = getNextSplit<float>(vectorStream, ',');

                float diffuse = getNextSplit<float>(lineStream, '/');
                float glass = getNextSplit<float>(lineStream, '/');
                float glassReflect = getNextSplit<float>(lineStream, '/');

                vectorStream = getNextStreamSplit(lineStream, '/');

                float uvX = getNextSplit<float>(vectorStream, ',');
                float uvY = getNextSplit<float>(vectorStream, ',');
                float uvWidth = getNextSplit<float>(vectorStream, ',');
                float uvHeight = getNextSplit<float>(vectorStream, ',');

                bool emissive = getNextSplit(lineStream, '/') == "true";

                materials.push_back(Game::Material(glm::vec3(red, green, blue), diffuse, glass, glassReflect, glm::vec4(uvX, uvY, uvWidth, uvHeight), emissive));
            }
            else if (readMode == BOX) {
                std::stringstream positionStream = getNextStreamSplit(lineStream, '/');
                std::stringstream scaleStream = getNextStreamSplit(lineStream, '/');

                float x = getNextSplit<float>(positionStream, ',');
                float y = getNextSplit<float>(positionStream, ',');
                float z = getNextSplit<float>(positionStream, ',');

                float width = getNextSplit<float>(scaleStream, ',');
                float height = getNextSplit<float>(scaleStream, ',');
                float length = getNextSplit<float>(scaleStream, ',');

                int material = getNextSplit<int>(lineStream, '/');
                std::string tag = getNextSplit(lineStream, '/');

                boxes.push_back(Game::Box(glm::vec3(x, y, z), glm::vec3(width, height, length), material, tag.c_str()));
            }
            else {
                std::stringstream positionStream = getNextStreamSplit(lineStream, '/');

                float x = getNextSplit<float>(positionStream, ',');
                float y = getNextSplit<float>(positionStream, ',');
                float z = getNextSplit<float>(positionStream, ',');

                float radius = getNextSplit<float>(lineStream, '/');

                int material = getNextSplit<int>(lineStream, '/');
                std::string tag = getNextSplit(lineStream, '/');

                spheres.push_back(Game::Sphere(glm::vec3(x, y, z), radius, material, tag.c_str()));
            }
        }
    }

    return Game::Map(albedoTexture, normalTexture, skyboxTexture, materials, boxes, spheres);
}

template<typename T> T Game::MapParser::parseString(std::string string) {
    T result;
    std::stringstream(string) >> result;

    return result;
}

std::string Game::MapParser::getNextSplit(std::stringstream& line, char splitter) {
    std::string result;
    std::getline(line, result, splitter);

    return result;
}
std::stringstream Game::MapParser::getNextStreamSplit(std::stringstream& line, char splitter) {
    std::string split = getNextSplit(line, splitter);
    return std::stringstream(split);
}

template<typename T> T Game::MapParser::getNextSplit(std::stringstream& line, char splitter) {
    return parseString<T>(getNextSplit(line, splitter));
}

float Game::World::gravity = 25.0f;
float* Game::World::sunDirection = new float[3];

Game::Map* Game::World::map = NULL;

void Game::World::initialize(const char* mapName, float gravity, glm::vec3 sunDirection) {
    map = new Map(MapParser::parse((std::string("res/maps/") + mapName + ".rtmap").c_str()));

    World::gravity = gravity;
    World::sunDirection[0] = sunDirection.x;
    World::sunDirection[1] = sunDirection.y;
    World::sunDirection[2] = sunDirection.z;
}
void Game::World::clear() {
    TT::Texture::clear(map->albedoTexture);
    TT::Texture::clear(map->normalTexture);
    TT::Texture::clear(map->skyboxTexture);

    delete map;
    delete[] sunDirection;
}

const float Game::Player::onGroundResetDelay = 0.3f;
const float Game::Player::stepSpeed = 0.5f;

Game::Player::Player(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
    walkSpeed = 6.0f;
    rotateSpeed = 0.09f;
    jumpHeight = 8.0f;
    eyeHeight = 0.2f;
    cinematicSharpness = 2.0f;

    stepTimer = 0.0f;
    onGroundResetDelayTimer = 0.0f;

    flyMode = false;
    cinematicMode = false;
    
    this->position = glm::vec3(position);
    this->rotation = glm::vec3(rotation);
    this->scale = glm::vec3(scale);

    rawRotation = glm::vec3(rotation);

    velocity = glm::vec3();
    startPosition = glm::vec3(position);
}

void Game::Player::respawn() {
    velocity = glm::vec3();
    position = glm::vec3(startPosition);
}
void Game::Player::update(TT::Time time) {
    velocity.x = 0.0f;
    velocity.z = 0.0f;

    if (!flyMode) velocity.y -= World::gravity * time.getDelta();
    else velocity.y = 0.0f;

    if (TT::Keyboard::isPressed(GLFW_KEY_W)) {
        velocity.x += sin(glm::radians(rotation.y));
        velocity.z += cos(glm::radians(rotation.y));
    }
    if (TT::Keyboard::isPressed(GLFW_KEY_S)) {
        velocity.x += sin(glm::radians(rotation.y + 180.0f));
        velocity.z += cos(glm::radians(rotation.y + 180.0f));
    }
    if (TT::Keyboard::isPressed(GLFW_KEY_D)) {
        velocity.x += sin(glm::radians(rotation.y + 90.0f));
        velocity.z += cos(glm::radians(rotation.y + 90.0f));
    }
    if (TT::Keyboard::isPressed(GLFW_KEY_A)) {
        velocity.x += sin(glm::radians(rotation.y - 90.0f));
        velocity.z += cos(glm::radians(rotation.y - 90.0f));
    }
    if (!flyMode) {
        if (TT::Keyboard::isPressed(GLFW_KEY_SPACE) && onGround) {
            velocity.y = jumpHeight;
            rawOnGround = false;
            onGround = false;
        }
    }
    else {
        if (TT::Keyboard::isPressed(GLFW_KEY_SPACE)) velocity.y += 1.0f;
        if (TT::Keyboard::isPressed(GLFW_KEY_LEFT_SHIFT))velocity.y -= 1.0f;
    }

    if (rawOnGround) {
        onGround = true;
        onGroundResetDelayTimer = 0.0f;
    }
    else {
        if (onGroundResetDelayTimer >= onGroundResetDelay) onGround = false;
        else onGroundResetDelayTimer += time.getDelta();
    }

    float horizontalLength = glm::length(glm::vec2(velocity.x, velocity.z));

    std::vector<std::string> collidedTags;

    rawOnGround = false;

    position.x += velocity.x * walkSpeed * time.getDelta();
    collidedTags.push_back(checkCollision(World::map->boxes, World::map->spheres));

    if (collidedTags[collidedTags.size() - 1] != "") {
        position.x -= velocity.x * walkSpeed * time.getDelta();
        velocity.x = 0.0f;
    }

    position.y += velocity.y * (flyMode ? walkSpeed : 1.0f) * time.getDelta();
    collidedTags.push_back(checkCollision(World::map->boxes, World::map->spheres));

    if (collidedTags[collidedTags.size() - 1] != "") {
        position.y -= velocity.y * (flyMode ? walkSpeed : 1.0f) * time.getDelta();

        if (velocity.y <= 0.0f) {
            bool onJumpPad = std::find(collidedTags.begin(), collidedTags.end(), "jump_pad") != collidedTags.end();
            velocity.y = onJumpPad ? 50.0f : 0.0f;

            if (!onJumpPad) rawOnGround = true;
        }
    }

    position.z += velocity.z * walkSpeed * time.getDelta();
    collidedTags.push_back(checkCollision(World::map->boxes, World::map->spheres));

    if (collidedTags[collidedTags.size() - 1] != "") {
        position.z -= velocity.z * walkSpeed * time.getDelta();
        velocity.z = 0.0f;
    }

    if (std::find(collidedTags.begin(), collidedTags.end(), "laser") != collidedTags.end())
        respawn();

    glm::vec3 lastRotation = glm::vec3(rotation);
    if (!cinematicMode) {
        rotation.x -= TT::Mouse::getVelocity().y * rotateSpeed;
        rotation.x = fmax(fmin(rotation.x, 89.99f), -89.99f);

        rotation.y += TT::Mouse::getVelocity().x * rotateSpeed;
        rotation.y -= floor(rotation.y / 360.0f) * 360.0f;
    }
    else {
        rawRotation.x -= TT::Mouse::getVelocity().y * rotateSpeed;
        rawRotation.x = fmax(fmin(rawRotation.x, 89.99f), -89.99f);
        rawRotation.y += TT::Mouse::getVelocity().x * rotateSpeed;

        rotation.x += (cinematicSharpness * time.getDelta()) * (rawRotation.x - rotation.x);
        rotation.y += (cinematicSharpness * time.getDelta()) * (rawRotation.y - rotation.y);
        rotation.z += (cinematicSharpness * time.getDelta()) * (rawRotation.z - rotation.z);
    }
}

void Game::Player::clear() {
    
}

glm::vec3 Game::Player::getEyePosition() {
    return glm::vec3(position.x + scale.x / 2.0f, position.y + scale.y - eyeHeight, position.z + scale.z / 2.0f);
}

std::string Game::Player::checkCollision(std::vector<Box> boxes, std::vector<Sphere> spheres) const {
    for (Box& box : boxes)
        if (position.x + scale.x >= box.position.x && position.x <= box.position.x + box.scale.x)
            if (position.y + scale.y >= box.position.y && position.y <= box.position.y + box.scale.y)
                if (position.z + scale.z >= box.position.z && position.z <= box.position.z + box.scale.z)
                    return box.tag;
    for (Sphere& sphere : spheres) {
        glm::vec3 nearest(glm::max(glm::min(sphere.position.x, position.x + scale.x), position.x), glm::max(glm::min(sphere.position.y, position.y + scale.y), position.y), glm::max(glm::min(sphere.position.z, position.z + scale.z), position.z));
        float length = glm::length(glm::vec3(sphere.position.x - nearest.x, sphere.position.y - nearest.y, sphere.position.z - nearest.z));

        if (length * length < sphere.radius * sphere.radius) return sphere.tag;
    }

    return "";
}

float Game::Camera::dofBlurSize = 0.05f;
float Game::Camera::dofFocusDistance = 12.0f;
float Game::Camera::fov = 90.0f;

void Game::Camera::initialize(float dofBlurSize, float dofFocusDistance, float fov) {
    Camera::dofBlurSize = dofBlurSize;
    Camera::dofFocusDistance = dofFocusDistance;
    Camera::fov = fov;
}

TT::ShaderProgram* Game::Renderer::renderProgram = NULL;
TT::ShaderProgram* Game::Renderer::screenProgram = NULL;

TT::FrameBuffer* Game::Renderer::firstFrameBuffer = NULL;
TT::FrameBuffer* Game::Renderer::secondFrameBuffer = NULL;

void Game::Renderer::initialize(glm::uvec2 size) {
    resize(size);
    reloadShaders();
}
void Game::Renderer::resize(glm::uvec2 size) {
    clearFrameBuffers();

    firstFrameBuffer = new TT::FrameBuffer(size.x, size.y);
    secondFrameBuffer = new TT::FrameBuffer(size.x, size.y);
}
void Game::Renderer::reloadShaders() {
    clearShaders();

    if (renderProgram) delete renderProgram;
    if (screenProgram) delete screenProgram;

    renderProgram = new TT::ShaderProgram();
    renderProgram->addShader(TT::Shader("res/shaders/render.vert", GL_VERTEX_SHADER));
    renderProgram->addShader(TT::Shader("res/shaders/render.frag", GL_FRAGMENT_SHADER));
    renderProgram->compile();

    screenProgram = new TT::ShaderProgram();
    screenProgram->addShader(TT::Shader("res/shaders/screen.vert", GL_VERTEX_SHADER));
    screenProgram->addShader(TT::Shader("res/shaders/screen.frag", GL_FRAGMENT_SHADER));
    screenProgram->compile();
}

void Game::Renderer::render(TT::Time time, Player player) {
    TT::FrameBuffer* renderFrameBuffer = secondFrameBuffer;
    TT::FrameBuffer* backFrameBuffer = firstFrameBuffer;

    renderFrameBuffer->load();

    renderProgram->load();
    renderProgram->setUniform("playerPosition", player.getEyePosition());
    renderProgram->setUniform("playerRotation", player.rotation);
    renderProgram->setUniform("sunDirection", glm::vec3(World::sunDirection[0], World::sunDirection[1], World::sunDirection[2]));
    renderProgram->setUniform("screenResolution", TT::Window::getSize());
    renderProgram->setUniform("backFrameFactor", 1.0f);
    renderProgram->setUniform("backFrameSampler", 0);
    renderProgram->setUniform("skyboxSampler", 1);
    renderProgram->setUniform("albedoSampler", 2);
    renderProgram->setUniform("normalSampler", 3);
    renderProgram->setUniform("fov", Camera::fov);

    for (int i = 0; i < World::map->boxes.size(); i++) {
        Box box = World::map->boxes[i];
        Material material = World::map->materials[box.material];

        std::string uniformId = "boxes[" + std::to_string(i) + ']';

        renderProgram->setUniform((uniformId + ".position").c_str(), box.position);
        renderProgram->setUniform((uniformId + ".size").c_str(), box.scale);
        renderProgram->setUniform((uniformId + ".material.color").c_str(), material.color);
        renderProgram->setUniform((uniformId + ".material.uvInfo").c_str(), material.uvInfo);
        renderProgram->setUniform((uniformId + ".material.emissive").c_str(), material.emissive ? 1 : 0);
    }
    for (int i = 0; i < World::map->spheres.size(); i++) {
        Sphere sphere = World::map->spheres[i];
        Material material = World::map->materials[sphere.material];

        std::string uniformId = "spheres[" + std::to_string(i) + ']';

        renderProgram->setUniform((uniformId + ".position").c_str(), sphere.position);
        renderProgram->setUniform((uniformId + ".radius").c_str(), sphere.radius);
        renderProgram->setUniform((uniformId + ".material.color").c_str(), material.color);
        renderProgram->setUniform((uniformId + ".material.uvInfo").c_str(), material.uvInfo);
        renderProgram->setUniform((uniformId + ".material.emissive").c_str(), material.emissive ? 1 : 0);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, backFrameBuffer->getTexture());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, World::map->skyboxTexture);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, World::map->albedoTexture);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, World::map->normalTexture);

    glBegin(GL_QUADS);
    glVertex2i(-1, -1);
    glVertex2i(1, -1);
    glVertex2i(1, 1);
    glVertex2i(-1, 1);
    glEnd();

    TT::FrameBuffer::unload();

    screenProgram->load();
    screenProgram->setUniform("screenResolution", TT::Window::getSize());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderFrameBuffer->getTexture());

    glBegin(GL_QUADS);
    glVertex2i(-1, -1);
    glVertex2i(1, -1);
    glVertex2i(1, 1);
    glVertex2i(-1, 1);
    glEnd();

    TT::ShaderProgram::unload();
    TT::Texture::unload();

    glBindTexture(GL_TEXTURE_2D, 0);
}
void Game::Renderer::clear() {
    clearShaders();
}
void Game::Renderer::clearShaders() {
    if(renderProgram) renderProgram->clear();
    if(screenProgram) screenProgram->clear();
}
void Game::Renderer::clearFrameBuffers() {
    if (firstFrameBuffer) {
        firstFrameBuffer->clear();
        delete firstFrameBuffer;
    }
    if (secondFrameBuffer) {
        secondFrameBuffer->clear();
        delete secondFrameBuffer;
    }
}

TT::FrameBuffer* Game::Renderer::getFirstFrameBuffer() {
    return firstFrameBuffer;
}
TT::FrameBuffer* Game::Renderer::getSecondFrameBuffer() {
    return secondFrameBuffer;
}