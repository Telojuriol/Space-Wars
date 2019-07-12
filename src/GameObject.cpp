#include "GameObject.h"
#include <string>
#include <iostream>
#include <algorithm>


void GameObject::addChild(GameObject* obj){

	obj->parent = this;

	this->children.push_back(obj);

};

void GameObject::removeChild(GameObject* obj){
	std::vector<GameObject*>::iterator i = std::remove(this->children.begin(),this->children.end(),obj);

	this->children.erase(i,this->children.end());

	delete(obj);
};

Matrix44 GameObject::getGlobalMatrix(){
	
	if(this->parent){
		return this->parent->getGlobalMatrix() * this->model;
	}else{
		return this->model;
	}


		
};

void GameObject::render(){
	for(int i = 0; i < this->children.size() ;i++){
		this->children[i]->render();

	}

};

void GameObject::update(float elapsed_time){
	for(int i = 0; i < this->children.size() ;i++){
		this->children[i]->update(elapsed_time);

	}

};

void GameObject::clear(){


};