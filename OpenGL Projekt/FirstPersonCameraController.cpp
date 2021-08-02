#include "FirstPersonCameraController.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <gtx/rotate_vector.hpp>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Collision.h"

#include "Player.h"
#include "World.h"

struct alignas(8) Test {
	int i : 1;
};

using namespace glm;

FirstPersonCameraController::FirstPersonCameraController(PerspectiveCamera* cam, World* world) : cam(cam), world(world), player(nullptr) {
	speed = 100;
	lastx = 0;
	lasty = 0;
}

void lightStuff(Block* b) {
	constexpr int idk[] = { 1, -1, 1, 0, 1, 1, 0, -1, 0, 1, -1, -1, -1, 0, -1, 1, 0, 0 };
	vector<DataChunk*> u;
	for(int i = 0; i < _countof(idk); i += 2) {
		Chunk* c = b->chunk->chunk->world->getChunk(b->chunk->chunk->x + idk[i], b->chunk->chunk->z + idk[i + 1], false);
		for(int j = 0; j < 3; j++) {
			DataChunk* dc = &c->chunks[b->chunk->y + j - 1];
			if(dist({ b->getAbsoluteX(), b->getAbsoluteY(), b->getAbsoluteZ() }, { dc->chunk->x, dc->y, dc->chunk->z }) < MAX_LIGHT_LEVEL) {
				b->chunk->chunk->world->lm.applyLight(dc);
				u.push_back(dc);
			}
		}
	}
	for(DataChunk* dc : u) {
		dc->meshNeedsUpdate();
	}
	b->chunk->updateMesh();
}

void FirstPersonCameraController::touchDown(int button) {
	for (float i = 0; i < 6; i += 0.1f) {
		vec3 p = cam->position + cam->direction * i;
		int rx = getRealBlockNumber(p.x);
		int rz = getRealBlockNumber(p.z);
		Block* b = world->getBlock(rx, (int) p.y, rz, false);
		if (b == nullptr) continue;
		if (b->material != &Material::AIR) {
			if (button == GLFW_MOUSE_BUTTON_LEFT) {
				b->material = &Material::AIR;
				std::cout << "Broke Block: " << b->getAbsoluteX() << " " << b->getAbsoluteY() << " " << b->getAbsoluteZ() << std::endl;
			}
			else if(button == GLFW_MOUSE_BUTTON_RIGHT) {
				p += cam->direction * -0.1f;
				rx = getRealBlockNumber(p.x);
				rz = getRealBlockNumber(p.z);
				b = world->getBlock(rx, (int) p.y, rz, false);
				if(b == nullptr) break;
				if(b->material != &Material::AIR) break;
				b->material = &Material::GRASS;
			}
			else continue;
			cout << "light it up " << endl;
			b->chunk->addLight({ b->getPosition(), Color::WHITE, 16 });
			TestSpeed(lightStuff(b));
			//b->chunk->meshNeedsUpdate();
			
			//TestSpeed(b->chunk->chunk->world->lm.applyLight(&world->getChunk(b->chunk->chunk->x, b->chunk->chunk->z, false)->chunks[b->chunk->y]));
			//TestSpeed(b->updateMesh(true));
			break;
		}
	}
}

std::ostream& operator<<(std::ostream& test, glm::vec3 v) {
	test << v.x << " " << v.y << " " << v.z;
	return test;
}

extern GLFWwindow* window;

void FirstPersonCameraController::update(float delta) {
	look = nullptr;
	for(float i = 0; i < 6; i += 0.1f) {
		vec3 p = cam->position + cam->direction * i;
		int rx = getRealBlockNumber(p.x);
		int rz = getRealBlockNumber(p.z);
		Block* b = world->getBlock(rx, (int) p.y, rz, false);
		if(b == nullptr) continue;
		if(b->material != &Material::AIR) {
			look = b;
			break;
		}
	}

	double xpos, ypos;

	static bool first = true;

	glfwGetCursorPos(window, &xpos, &ypos);

	if (first) {
		first = false;
		lastx = xpos;
		lasty = ypos;
		return;
	}

	if (xpos != lastx || ypos != lasty) {
		float movedx = (float)(lastx - xpos);
		float movedy = (float)(lasty - ypos);
		lastx = xpos;
		lasty = ypos;


		glm::vec3 before = cam->direction;
		vec3 t = normalize(cross(cam->direction, cam->up));

		cam->direction = rotateY(cam->direction, movedx / 600.f);

		cam->direction = rotateX(cam->direction, movedy / 600.f * t.x);
		cam->direction = rotateZ(cam->direction, movedy / 600.f * t.z);

		if ((cam->direction.x > 0) ^ (before.x > 0) && (cam->direction.z > 0) ^ (before.z > 0)) {
			cam->direction = before;
		}

	}

	player->triesToMove = false;
	Vector3G moveDir(0, 0, 0);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		player->triesToMove = true;
		moveDir += normalize(vec3(cam->direction.x, 0, cam->direction.z));
		
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		player->triesToMove = true;
		moveDir -= normalize(vec3(cam->direction.x, 0, cam->direction.z));
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		if(player->fly) {
			player->velocity.y = speed;
		} else
		if (player->isOnGround()) {
			player->velocity.y = 9.5f;
		}
	} else
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		if(player->fly) {
			player->velocity.y = -speed;
		}
	}
	else if(player->fly) {
		player->velocity.y = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		player->triesToMove = true;
		moveDir -= normalize(cross(cam->direction, cam->up));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		player->triesToMove = true;
		moveDir += normalize(cross(cam->direction, cam->up));
	}

	if (moveDir.x || moveDir.y || moveDir.z) {
		moveDir = normalize(moveDir);
		float cspeed = player->velocity.x + player->velocity.z;
		float p = cspeed / speed;
		Vector3G mm = normalize(vec3(moveDir.x, 0, moveDir.z)) * speed;
		Vector3G ns = mm * (speed * delta * 0.5f);
		player->velocity.x += ns.x;
		player->velocity.z += ns.z;
		if (abs(player->velocity.x) > abs(mm.x)) {
			player->velocity.x = mm.x;
		}
		if (abs(player->velocity.z) > abs(mm.z)) {
			player->velocity.z = mm.z;
		}
	}

	player->lookingAt = nullptr;

	for (float i = 0; i < 6; i += 0.1f) {
		vec3 p = cam->position + cam->direction * i;
		int rx = getRealBlockNumber(p.x);
		int rz = getRealBlockNumber(p.z);
		Block* b = world->getBlock(rx, (int)p.y, rz, false);
		if (b == nullptr) continue;
		if (b->material != &Material::AIR) {
			player->lookingAt = b;
			break;
		}
	}
}
