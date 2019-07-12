#pragma once
#include <iostream>
#include "framework.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Shader.h"
#include "scenes.h"

class BulletManager{
public:

	struct Bullet{
		bool valid;
		Vector3 position;
		Vector3 old_position;
		Vector3 velocity;
		float ttl;
		float power;
		GameObject* owner;

	};

	Mesh* bullet_mesh;
	std::vector<Bullet> bullets_vector;
	Shader* shader;
	bool can_fire;
	
	BulletManager(Mesh* daMesh, int max_bullets);

	void render();
	void update(float elapsed_time);
	void createBullet(Vector3 pos, Vector3 vel, float power, float ttl, GameObject* owner);
	void checkCollisions(std::vector<GameObject*>& colliders);

};
