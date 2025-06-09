#pragma once

#include "shader.hpp"

class BlackHoleShader: public Shader {
public:
    BlackHoleShader():
        Shader(
            "../../shaders/blackHoleShader.vert",
            "../../shaders/blackHoleShader.frag"
        ) {}

    void InverseViewMatrix(const glm::mat4& matrix) const
        { SetMatrix4("invViewMat", matrix); }

    void InversePerspectiveMatrix(const glm::mat4& matrix) const
        { SetMatrix4("invPersMat", matrix); }

    // void Resolution(const glm::vec2& resolution) const
    //     { SetVec2("resolution",  resolution); }

    void BlackHoleMass(const float mass) const
        { SetFloat("blackHoleMass", mass); }

    void CameraPos(const glm::vec3& position) const
        { SetVec3("cameraPos", position); }
};
