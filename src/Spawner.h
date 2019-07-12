#pragma once

#include <iostream>
#include "framework.h"
#include "enemyController.h"


class Spawner : public GameObjectMesh{
public:
	
	Spawner();
	~Spawner();
	
	void update(float elapsed_time);

};
