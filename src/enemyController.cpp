#include "enemyController.h"
#include "scenes.h"
#include "managers.h"
enemyController::enemyController(){
	//model.setRotation(90 * DEG2RAD, Vector3(1,0,0) );
	lifePoints = 30;

	int randWayP = (rand()%(50));
	target_pos = wayPoints[randWayP];
	
}

float time1 = 0;
float time2 = 0;

enemyController::~enemyController(){



}


float speed = 20;
bool attackPlayer = false;

void enemyController::update(float elapsed_time){
	
	if(renderable){
		time2 += elapsed_time;
		f = this->model.frontVector().normalize();
		Vector3 object_pos(this->model.m[12],this->model.m[13],this->model.m[14]);

		Vector3 to_target = target_pos - object_pos;

		float distance = to_target.length();

		f = this->model.frontVector().normalize();
		this->model.traslate(to_target.normalize().x * elapsed_time * speed,to_target.normalize().y * elapsed_time * speed,to_target.normalize().z* elapsed_time * speed);
		Vector3 playerPos = Vector3(scene1.camera->eye) + scene1.camera->center * -10 * (50 *((float)(rand()%(20+20 + 1) -20)/10));

		float distToPlayer = (playerPos-object_pos).length();
		if(distToPlayer < 1000){
			attackPlayer = true;

		}else{
			attackPlayer = false;


		}

		if(attackPlayer){

			target_pos = playerPos;
			if(time2 - time1 > 2){
				g_bulletManager->createBullet(Vector3(this->model.m[12],this->model.m[13],this->model.m[14]) , ((playerPos-object_pos).normalize() * 0.1) * 30 , 20, 20.0,this);
				time1 = time2;
			}

		}else{

			if(distance < 200){
				int randWayP = (rand()%(50) );
				target_pos = wayPoints[randWayP];
				std::cout <<"num random Waypoint1:  "<< randWayP <<"    posX:  "<<wayPoints[randWayP].x  <<"    posY:  "<<wayPoints[randWayP].y <<"    posZ:  "<<wayPoints[randWayP].z<< "   dist  "  <<  (target_pos - object_pos).length() <<std::endl;
			}

		}

	}
	
	
}



