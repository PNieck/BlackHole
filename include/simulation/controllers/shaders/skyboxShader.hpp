#pragma once

#include "shader.hpp"


class SkyBoxShader: public Shader {
public:
    SkyBoxShader():
        Shader(
            "../../shaders/skyBoxShader.vert",
            "../../shaders/skyBoxShader.frag"
        ) {}

    void SetProjectionMatrix(const glm::mat4& matrix) const
        { SetMatrix4("projection", matrix); }

    void SetViewMatrix(const glm::mat4& matrix) const
        { SetMatrix4("view", glm::mat4(glm::mat3(matrix))); }
};