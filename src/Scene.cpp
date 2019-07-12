#include "Scene.h"
#include "managers.h"
Scene::Scene(){
	this->root = new GameObject();
	this->root->name = "root";
	this->root->parent = NULL;

}

Scene::~Scene(){
	this->root->clear();
	delete this->root;

}

void Scene::render(){
	this->root->render();

}

void Scene::update(float elapsed_time){
	this->root->update(elapsed_time);
	g_bulletManager->update(elapsed_time);
}


void Scene::getColliders(std::vector<GameObject*>& colliders, GameObject* current){
	if(!current->is_collider){
		//std::cout << current->name << std::endl;
		colliders.push_back(current);
	}
	for(int i = 0; i < current->children.size(); i++){
		getColliders(colliders, current->children[i]);
	}

}