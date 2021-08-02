#pragma once

#include "Texture.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "FirstPersonCameraController.h"

#include <string>


unsigned int createCubemap(TextureRegion tr[6]);

class Skybox {
	unsigned int id;
	VertexData vertices;
	ShaderProgram shader;
	FirstPersonCameraController* cam;

public:
	Skybox(std::string texturePath[6], FirstPersonCameraController* cam);
	void draw();
	~Skybox();
};