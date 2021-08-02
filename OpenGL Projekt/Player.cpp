#include "Player.h"

Player::Player(World* world, float x, float y, float z) : Entity(world, ColliderType::Dynamic, x, y, z, 0.7f, 1.8f, 0.7f), cam(110, 16, 9), controller(&cam, world), lookingAt(nullptr) {
	cam.position = position;
	cam.near = 0.1f;
	cam.far = 1000;
	controller.player = this;
}

float t = 0;

void Player::update(float delta) {
	if(fly) velocity += world->gravity * delta;
	controller.update(delta);
	Vector3G dir = glm::normalize(Vector3G(velocity.x, 0, velocity.z));
	float v;
	if (velocity.x || velocity.z) {
		if (!triesToMove && (isOnGround() || fly)) {
			velocity.x = ((v = abs(velocity.x) - abs(dir.x) * friction.x * delta) < 0 ? 0 : v) * (velocity.x < 0 ? -1 : 1);
			velocity.z = ((v = abs(velocity.z) - abs(dir.z) * friction.z * delta) < 0 ? 0 : v) * (velocity.z < 0 ? -1 : 1);
		}
	}

	Vector3G move = velocity * delta;
	glm::vec3 moved = world->move(this, move.x, move.y, move.z);
	Vector3G r = (move - moved);
	if (moved.x == 0) velocity.x = 0;
	if (moved.y == 0) velocity.y = 0;
	if (moved.z == 0) velocity.z = 0;
	//velocity.x *= move.x / r.x;
	//velocity.y *= move.y / r.y;
	//velocity.z *= move.z / r.z;
	cam.position = position;
	cam.position.y += collider.height - collider.height / 10;
	collider.x = position.x - collider.width / 2;
	collider.y = position.y;
	collider.z = position.z - collider.depth / 2;
	cam.update();
}