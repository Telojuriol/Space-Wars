#pragma once

#include <iostream>
#include "framework.h"
#include "texture.h"
#include "mesh.h"
#include "shader.h"
#include "GameObject.h"


class GameObjectLight : public GameObject{
public:
	Vector3 position;

	GameObjectLight();
	GameObjectLight(Vector3 position);
	~GameObjectLight();

	void render();
	void update(float elapsed_time);


};