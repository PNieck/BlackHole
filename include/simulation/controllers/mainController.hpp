#pragma once

#include "mouseState.hpp"
#include "cubeMap.hpp"
#include "mesh.hpp"
#include "camera.hpp"
#include "shaders/skyboxShader.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


class MainController {
public:
    explicit MainController(int width, int height, GLFWwindow* window);
    ~MainController();

    void Render();

    void SizeChanged(int width, int height);
    void MouseClicked(MouseButton button);
    void MouseReleased(MouseButton button);
    void MouseMoved(float x, float y);
    void ScrollMoved(int offset);

private:
    MouseState mouseState;

    Camera camera;
    SkyBoxShader skyboxShader;

    Mesh cubeMesh;
    CubeMap cubeMap;

    void UpdateCubeMesh();

    void RotateCamera(float x, float y);
};
