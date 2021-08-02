#include "Cubemap.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Texture.h"

#include "Utils.h"

unsigned int createCubemap(Texture t[6]) {
    unsigned int id;
    GLCall(glGenTextures(1, &id));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, id));
    for (int i = 0; i < 6; i++) {
        Texture& ct = t[i];
        GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, ct.getWidth(), ct.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, ct.getDataBuffer()));
    }
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    return id;
}

float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

Skybox::Skybox(std::string texturePath[6], FirstPersonCameraController* cam) : cam(cam), vertices(36, VertexAttribute(3) ), shader("assets\\el\\shader\\Skybox\\VertexShader.glsl", "assets\\el\\shader\\Skybox\\FragmentShader.glsl", true) {
    Texture textures[6] {
        {texturePath[0], false},
        {texturePath[1], false},
        {texturePath[2], false},
        {texturePath[3], false},
        {texturePath[4], false},
        {texturePath[5], false},
    };

    vertices.setVertices(skyboxVertices, 36);

    id = createCubemap(textures);
}

void Skybox::draw() {
    GLCall(glDepthFunc(GL_LEQUAL));
    shader.bind();
    shader.setUniformMat4f("matrix", cam->getCamera()->proj * glm::mat4(glm::mat3(cam->getCamera()->view)));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, id));
    vertices.draw(GL_TRIANGLES);
    GLCall(glDepthFunc(GL_LESS));
}

Skybox::~Skybox() {
    GLCall(glDeleteBuffers(1, &id));
}