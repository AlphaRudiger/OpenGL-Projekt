#pragma once

#include "Utils.h"
#include "World.h"
#include "Player.h"

void raycast(Player* p) {
	Vector3G dir = p->cam.direction;
	Vector3G currentPos = p->cam.position;

	cout << "SC: " << 1 / dir.x << endl;
}
