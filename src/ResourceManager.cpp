#include "ResourceManager.h"

using namespace std;

ResourceManager::ResourceManager()
{
	
}


ResourceManager::~ResourceManager()
{
	
}

void ResourceManager::loadMesh(const string handle, const string path){
	Mesh *mesh = new Mesh();
	mesh->loadASE(path.c_str());
	mesh->setCollisionModel();
	m_meshes[handle] = mesh;

}

void ResourceManager::loadTexture(const string handle, const string path){
	Texture *texture = new Texture();
	texture->load(path.c_str());
	m_textures[handle] = texture;
}

void ResourceManager::loadShader(const string handle, const string vertexPath, const string fragmentPath){
	Shader *shader = new Shader();
	shader->load(vertexPath, fragmentPath);
	m_shaders[handle] = shader;

}

void ResourceManager::unloadMesh(const string handle){
	std::unordered_map<std::string, Mesh*>::iterator itr = m_meshes.find(handle);
	if(itr != m_meshes.end()){
		delete itr->second;
		m_meshes.erase(itr);

	}else{
		cout << "could not find the mesh with that handle" << handle << endl;
		exit(0);

	}

}

void ResourceManager::unloadTexture(const string handle){
	std::unordered_map<std::string, Texture*>::iterator itr = m_textures.find(handle);
	if(itr != m_textures.end()){
		delete itr->second;
		m_textures.erase(itr);

	}else{
		cout << "could not find the texture with that handle" << handle << endl;
		exit(0);

	}

}

void ResourceManager::unloadShader(const string handle){
	std::unordered_map<std::string, Shader*>::iterator itr = m_shaders.find(handle);
	if(itr != m_shaders.end()){
		delete itr->second;
		m_shaders.erase(itr);

	}else{
		cout << "could not find the shader with that handle" << handle << endl;
		exit(0);

	}

}

Mesh* ResourceManager::getMesh(const string handle){
	return m_meshes.at(handle);

}


Texture* ResourceManager::getTexture(const string handle){
	return m_textures.at(handle);

}


Shader* ResourceManager::getShader(const string handle){
	return m_shaders.at(handle);

}
