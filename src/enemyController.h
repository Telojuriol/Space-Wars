#pragma once

#include <iostream>
#include "GameObjectMesh.h"


class enemyController : public GameObjectMesh{
public:


	enemyController();
	~enemyController();

	void update(float elapsed_time);


private:
	Vector3 target_pos;
	Vector3 f;
};