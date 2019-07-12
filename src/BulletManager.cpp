#include "BulletManager.h"
#include "GameObjectMesh.h"

BulletManager::BulletManager(Mesh* daMesh, int max_bullets){
	bullet_mesh = daMesh;
	bullets_vector.resize(max_bullets);

}

void BulletManager::render()
{
	for(int i = 0; i < bullets_vector.size();i++){
		if(bullets_vector[i].valid){
			Camera* camera = scene1.camera;
		Vector3 light = scene1.lights[0]->position;
		Matrix44 model;
		model.setTranslation(bullets_vector[i].position.x,bullets_vector[i].position.y,bullets_vector[i].position.z);
		Matrix44 mvp = model * camera->viewprojection_matrix;
		Matrix44 inverse_m = model;
		inverse_m.inverse();
		Matrix44 normal_m = inverse_m.transpose();
		shader->enable();
		shader->setMatrix44("u_mvp", mvp );
		shader->setMatrix44("u_normal_matrix", normal_m );
		shader->setUniform3("u_light",light);
		//shader->setTexture("u_texture_diffuse",texture_diffuse);
		//shader->setTexture("u_texture_specular",texture_specular);
		shader->setUniform3("u_camera_position", Vector3(0,25,25));
		bullet_mesh->render(GL_TRIANGLES, shader);

		shader->disable();

		}
		


	}


}
void BulletManager::update(float elapsed_time){
	for(int i = 0; i < bullets_vector.size();i++){
		if(bullets_vector[i].valid){
			bullets_vector[i].old_position = bullets_vector[i].position;
			bullets_vector[i].position = bullets_vector[i].position + bullets_vector[i].velocity;
			bullets_vector[i].ttl -= elapsed_time;
			if(bullets_vector[i].ttl < 0.0f){
				bullets_vector[i].valid = false;
			}
			//Test collisiona
		}

	}

}

	//std::cout << "patata" << std::endl;
void BulletManager::createBullet(Vector3 pos, Vector3 vel, float power, float ttl, GameObject* owner){
	Bullet b;
	b.position = pos;
	b.velocity = vel;
	b.power = power;
	b.ttl = ttl;
	b.owner = owner;
	b.valid = true;

	for(int i = 0; i < bullets_vector.size();i++){
		if(!bullets_vector[i].valid){
			bullets_vector[i] = b;
			break;
		}

	}



}


void BulletManager::checkCollisions(std::vector<GameObject*>& colliders){
	for(int i = 0; i < bullets_vector.size();i++){

		if(!bullets_vector[i].valid){
			continue;
		}

		for(int j = 0; j < colliders.size(); j++){

			if(bullets_vector[i].owner == colliders[j]){
				continue;
			}

			GameObjectMesh* go = dynamic_cast<GameObjectMesh*>(colliders[j]);
			
			go->mesh->collision_model->setTransform(go->model.m);
			

			if(go->mesh->collision_model->rayCollision(bullets_vector[i].old_position.v , bullets_vector[i].position.v , false, 0 ,1)){


				bullets_vector[i].valid = false;

				Vector3 collision_point;
				go->mesh->collision_model->getCollisionPoint(collision_point.v,true);

				go->onCollideBullet(bullets_vector[i].power, collision_point);



			}

		
		}



	}


}
