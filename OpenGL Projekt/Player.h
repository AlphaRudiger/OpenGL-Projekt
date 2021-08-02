#pragma once

#include "PerspectiveCamera.h"
#include "FirstPersonCameraController.h"
#include "Collision.h"
#include "Entity.h"

class Player : public Entity {

public:
	bool fly = true;
	Block* lookingAt;
	PerspectiveCamera cam;

	FirstPersonCameraController controller;

	Player(World* world, float x, float y, float z);

	void update(float delta) override;
};