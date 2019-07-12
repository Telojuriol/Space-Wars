#pragma once

#include <iostream>
#include "framework.h"
#include "texture.h"
#include "mesh.h"
#include "shader.h"
#include "GameObject.h"
#include "camera.h"

class GameObjectEnvironment : public GameObject{
public:
	Mesh* mesh;
	Texture* texture;
	Shader* shader;
	Vector3 color;

	GameObjectEnvironment();
	GameObjectEnvironment(Mesh* m,Shader* s,Texture* t);
	~GameObjectEnvironment();

	//void render();
	void render();
	void update(float elapsed_time);


};
