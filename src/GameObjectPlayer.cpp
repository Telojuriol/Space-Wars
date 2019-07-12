#include "GameObjectPlayer.h"
#include "scenes.h"
#include "game.h"
#include "managers.h"

Game* game_i = Game::instance;

GameObjectPlayer::GameObjectPlayer(){
	


}




GameObjectPlayer::~GameObjectPlayer(){



}

float distCameratoPlayer = 70.0f;
Vector3 fw_camera;
float lerpFactor = 0;
bool shot1 = false;
bool shot2 = false;

void GameObjectPlayer::update(float elapsed_time){
	movement(elapsed_time);
}


float GameObjectPlayer::lerp(float a, float b, float f) 
{
    return (a * (1.0f - f)) + (b * f);
}

float rotFactor = 1.4;
void GameObjectPlayer::movement(float elapsed_time){
	float speed = 2;
	Vector3 fw = this->model.frontVector();
	Vector3 player_pos = Vector3(this->model.m[12],this->model.m[13],this->model.m[14]);
	float yMovement = 0;

	if(Game::instance->keystate[SDL_SCANCODE_W] || Game::instance->keystate[SDL_SCANCODE_UP]){

		this->model.rotateLocal(elapsed_time * rotFactor, Vector3(1,0,0));
		yMovement = 1;
	}
	if(Game::instance->keystate[SDL_SCANCODE_D] || Game::instance->keystate[SDL_SCANCODE_RIGHT]){
		this->model.rotateLocal(-elapsed_time * rotFactor, Vector3(0,0,1));
	}
	if(Game::instance->keystate[SDL_SCANCODE_S] || Game::instance->keystate[SDL_SCANCODE_DOWN]){
		this->model.rotateLocal(-elapsed_time * rotFactor, Vector3(1,0,0));
		yMovement = -1;
	}
	if(Game::instance->keystate[SDL_SCANCODE_A] || Game::instance->keystate[SDL_SCANCODE_LEFT]){
		this->model.rotateLocal(elapsed_time * rotFactor, Vector3(0,0,1));
	}
	if(Game::instance->keystate[SDL_SCANCODE_LSHIFT]){
		speed = 4;
	}
	if(Game::instance->keystate[SDL_SCANCODE_LCTRL]){
		speed = 1;
	}


	fw = this->model.frontVector();
	model.traslate(fw.x * speed,fw.y * speed,fw.z * speed);


	Vector3 up_model(this->model.m[4],this->model.m[5],this->model.m[6]);

	scene1.camera->eye = Vector3(this->model.m[12]-fw.x* distCameratoPlayer,this->model.m[13]-fw.y* distCameratoPlayer,this->model.m[14]-fw.z* distCameratoPlayer) + up_model*20;
	lerpFactor = lerp(lerpFactor , yMovement * 0.1 , 0.05);
	fw_camera = scene1.camera->eye + fw + (up_model * lerpFactor);
	scene1.camera->lookAt(scene1.camera->eye, fw_camera,up_model);
	

	//Shot Control
	shot2 = false;
	if(Game::instance->keystate[SDL_SCANCODE_SPACE]){
		shot2 = true;
		
	}

	if(shot2 && !shot1){
		g_bulletManager->createBullet(Vector3(this->model.m[12],this->model.m[13],this->model.m[14]) , (fw + up_model * 0.1) * 30 , 20, 20.0,this);
	}

	shot1 = shot2;
}

