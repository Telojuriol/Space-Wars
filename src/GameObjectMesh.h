#pragma once

#include <iostream>
#include "framework.h"
#include "texture.h"
#include "mesh.h"
#include "shader.h"
#include "GameObject.h"


class GameObjectMesh : public GameObject{
public:
	Mesh* mesh;
	Texture* texture_diffuse;
	Texture* texture_specular;
	Shader* shader;
	Vector3 color;
	float lifePoints;
	bool renderable;

	GameObjectMesh();
	GameObjectMesh(Mesh* m,Shader* s,Texture* t_diffuse,Texture* t_specular);
	~GameObjectMesh();
	

	void render();
	void update(float elapsed_time);
	void onCollideBullet(float power,Vector3 collision_point);

};

