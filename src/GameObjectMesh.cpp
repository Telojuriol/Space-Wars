#include "GameObjectMesh.h"
#include "scenes.h"
#include "managers.h"
GameObjectMesh::GameObjectMesh(){



}


GameObjectMesh::GameObjectMesh(Mesh* m,Shader* s,Texture* t_diffuse,Texture* t_specular){
	mesh = m;
	texture_diffuse = t_diffuse;
	texture_specular = t_specular;
	shader = s;
	color = Vector3(1.0f,0.0f,0.0f);
	renderable = true;
	//model.setRotation(90 * DEG2RAD, Vector3(1,0,0) );
	this->parent = NULL;
}

GameObjectMesh::~GameObjectMesh(){



}

void GameObjectMesh::onCollideBullet(float power,Vector3 collision_point){

	lifePoints -= power;
	if(lifePoints <= 0){
		if(this->name.compare("Player") != 0){
			renderable = false;
			this->is_collider = true;
		}

	}


}

void GameObjectMesh::render(){
	if(renderable){
		for(int i = 0; i < this->children.size() ;i++){
			this->children[i]->render();

		}
		Camera* camera = scene1.camera;
		Vector3 light = scene1.lights[0]->position;

		Matrix44 mvp = this->getGlobalMatrix() * camera->viewprojection_matrix;
		Matrix44 inverse_m = model;
		inverse_m.inverse();
		Matrix44 normal_m = inverse_m.transpose();
		shader->enable();
		shader->setMatrix44("u_mvp", mvp );
		shader->setMatrix44("u_normal_matrix", normal_m );
		shader->setUniform3("u_light",light);
		shader->setTexture("u_texture_diffuse",texture_diffuse);
		shader->setTexture("u_texture_specular",texture_specular);
		shader->setUniform3("u_camera_position", Vector3(0,25,25));
		mesh->render(GL_TRIANGLES, shader);

		shader->disable();
	}
}

void GameObjectMesh::update(float elapsed_time){
	
	//model.traslate(30 * elapsed_time,0,0);
	//model.rotate(90 * elapsed_time * DEG2RAD, Vector3(0,1,0) );
	
}





