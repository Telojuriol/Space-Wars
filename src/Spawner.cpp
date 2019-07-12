#include "Spawner.h"
#include "scenes.h"
#include "managers.h"
#include "enemyController.h"
Spawner::Spawner(){



}




Spawner::~Spawner(){



}


float time10 = 0;
float time20 = 0;

void Spawner::update(float elapsed_time){
	time20 += elapsed_time;

	if(time20 - time10 > 15){
		enemyController* new_enemy = new enemyController();
		new_enemy->mesh = g_resourceManager.getMesh("x3_fighter");
		new_enemy->texture_diffuse = g_resourceManager.getTexture("x3_fighter_diffuse");
		new_enemy->texture_specular = g_resourceManager.getTexture("x3_fighter_specular");
		new_enemy->shader = g_resourceManager.getShader("bulletShader");
		new_enemy->is_collider = false;
		new_enemy->model.setTranslation(this->model.m[12],this->model.m[13],this->model.m[14]);
		scene1.root->addChild(new_enemy);

		time10 = time20;
	}
	
	
}



