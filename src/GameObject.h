#pragma once
#include <iostream>
#include "framework.h"
#include "camera.h"

class GameObject{
public:
	std::string name;
	Matrix44 model;

	bool is_collider;

	GameObject* parent;
	std::vector<GameObject*> children;

	virtual void render();
	virtual void update(float elapsed_time);

	void addChild(GameObject* obj);
	void removeChild(GameObject* obj);
	void clear();

	Matrix44 getGlobalMatrix();

};

