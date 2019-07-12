#pragma once

#include <iostream>
#include "framework.h"
#include "GameObject.h"
#include "GameObjectLight.h"
#include "GameObjectPlayer.h"

class Scene{
public:
	

	Camera* camera;
	GameObject* root;
	std::vector<GameObjectLight*> lights;
	Scene();
	~Scene();

	void render();
	void update(float elapsed_time);

	void getColliders(std::vector<GameObject*>& colliders, GameObject* current);
};