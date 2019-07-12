#pragma once

#include <iostream>
#include "GameObjectMesh.h"


class GameObjectPlayer : public GameObjectMesh{
public:

	GameObjectPlayer();
	~GameObjectPlayer();

	void update(float elapsed_time);

	float lerp(float a, float b, float f);

private:

	void movement(float elapsed_time);
};

