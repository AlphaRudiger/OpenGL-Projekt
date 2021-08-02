#pragma once

#include "PerspectiveCamera.h"
#include "Input.h"
#include "World.h"

class FirstPersonCameraController : public InputProcessor {
private:
	double lastx, lasty;
	PerspectiveCamera* cam;
	World* world;

public:
	Block* look = nullptr;
	Player* player;
	float speed;
	FirstPersonCameraController(PerspectiveCamera* cam, World* world);

	void touchDown(int button) override;
	void update(float delta);
	inline PerspectiveCamera* getCamera() {
		return cam;
	}
};