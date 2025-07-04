#include <simulation/controllers/cubeMap.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <stdexcept>


CubeMap::CubeMap(const std::string &posXTex, const std::string &negXTex, const std::string &posYTex, const std::string &negYTex, const std::string &posZTex, const std::string &negZTex)
{
    glActiveTexture(GL_TEXTURE0);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    SetTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X, posXTex);
    SetTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, negXTex);

    SetTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, posYTex);
    SetTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, negYTex);

    SetTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, posZTex);
    SetTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, negZTex);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}


CubeMap::~CubeMap() {
    glDeleteTextures(1, &textureID);
}


void CubeMap::SetTexture(const GLenum texType, const std::string &path) const
{
    int texWidth, texHeight, channels;
    unsigned char* data = stbi_load(path.c_str(), &texWidth, &texHeight, &channels, STBI_rgb);
    if (data == nullptr)
        throw std::runtime_error("Error while loading a texture");

    glTexImage2D(
        texType,
        0,          // default mipmap level
        GL_RGB,
        texWidth,
        texHeight,
        0,          // zero for legacy reasons
        GL_RGB,
        GL_UNSIGNED_BYTE,
        data
    );

    stbi_image_free(data);
}
