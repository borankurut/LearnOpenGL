#pragma once
#include "camera.hpp"

class Camera;

class WindowContext{
public:
    WindowContext(Camera* camera) : camera(camera) {}

    Camera* camera;
};

