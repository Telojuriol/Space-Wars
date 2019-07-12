#include "GameObjectEnvironment.h"
#include "scenes.h"
GameObjectEnvironment::GameObjectEnvironment(){


}

GameObjectEnvironment::~GameObjectEnvironment(){


}

GameObjectEnvironment::GameObjectEnvironment(Mesh* m,Shader* s,Texture* t){
	mesh = m;
	texture = t;
	shader = s;
	color = Vector3(1.0f,0.0f,0.0f);
	
}

void GameObjectEnvironment::render(){
	

	glDisable(GL_DEPTH_TEST);
	Camera* camera = scene1.camera;
	model.setTranslation(camera->eye.x,camera->eye.y,camera->eye.z);
	model.rotateLocal(90,Vector3(1,0,0));
	Matrix44 mvp = model * camera->viewprojection_matrix;
	Matrix44 inverse_m = model;
	inverse_m.inverse();
	Matrix44 normal_m = inverse_m.transpose();

	shader->enable();
	shader->setMatrix44("u_mvp", mvp );
	shader->setTexture("u_texture_diffuse",texture);

	glCullFace(GL_FRONT);
	mesh->render(GL_TRIANGLES, shader);
	glCullFace(GL_BACK);
	shader->disable();

	glEnable(GL_DEPTH_TEST);
	
}

void GameObjectEnvironment::update(float elapsed_time){
	
}


