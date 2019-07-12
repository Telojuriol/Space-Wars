#include "LevelParser.h"
#include "scenes.h"
#include "managers.h"
#include "GameObjectEnvironment.h"
#include "GameObjectMesh.h"
#include "GameObjectLight.h"
#include "camera.h"
#include "game.h"
#include "enemyController.h"
#include "GameObjectPlayer.h"
#include "Spawner.h"

void LevelParser::loadLevel(char* level_url){

	Game* game = Game::instance;

	rapidxml::file<> xmlFile(level_url);
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());

	xml_node<> *scene = doc.first_node();

	//Get All resources
	xml_node<> *resources = scene->first_node("resources");
	xml_node<> *resource_meshes = resources->first_node("meshes");

	for(xml_node<> *mesh = resource_meshes->first_node() ; mesh ; mesh = mesh->next_sibling()){
		g_resourceManager.loadMesh(mesh->first_node("name")->value(),mesh->first_node("url")->value());

	}

	xml_node<> *resource_textures = resources->first_node("textures");

	for(xml_node<> *texture = resource_textures->first_node() ; texture ; texture = texture->next_sibling()){
		g_resourceManager.loadTexture(texture->first_node("name")->value(),texture->first_node("url")->value());

	}

	xml_node<> *resource_shaders = resources->first_node("shaders");

	for(xml_node<> *shader = resource_shaders->first_node() ; shader ; shader = shader->next_sibling()){
		g_resourceManager.loadShader(shader->first_node("name")->value(),shader->first_node("url-vert")->value(),shader->first_node("url-frag")->value());
	}

	//Get All scene
	xml_node<> *root = scene->first_node("root");
	//ENVIRONMENT
	xml_node<> *environment = root->first_node("environment");

	GameObjectEnvironment* environ;
	environ = new GameObjectEnvironment();
	environ->name = environment->first_node("name")->value();
	environ->mesh = g_resourceManager.getMesh(environment->first_node("mesh")->value());
	environ->shader = g_resourceManager.getShader(environment->first_node("shader")->value());
	environ->texture = g_resourceManager.getTexture(environment->first_node("texture")->value());
	xml_node<> *transform = environment->first_node("transform");

	for(xml_node<> *attributes = transform->first_node() ; attributes ; attributes = attributes->next_sibling()){
		
		if(strcmp(attributes->name(),"scale") == 0){
			float sx = (float) atof(attributes->first_node("x")->value());
			float sy = (float) atof(attributes->first_node("y")->value());
			float sz = (float) atof(attributes->first_node("z")->value());
			environ->model.scale(sx,sy,sz);
		}


	}

	scene1.root->addChild(environ);

	//LIGHT
	xml_node<> *light = root->first_node("light");
	xml_node<> *light_position = light->first_node("position");
	float lpx = (float) atof(light_position->first_node("x")->value());
	float lpy = (float) atof(light_position->first_node("y")->value());
	float lpz = (float) atof(light_position->first_node("z")->value());
	GameObjectLight* light_go = new GameObjectLight(Vector3(lpx,lpy,lpz));

	scene1.lights.push_back(light_go);
	
	//CAMERA

	xml_node<> *camera = root->first_node("camera");
	float fov_camera = (float) atof(camera->first_node("fov")->value());
	float near_camera = (float) atof(camera->first_node("near")->value());
	float far_camera = (float) atof(camera->first_node("far")->value());

	xml_node<> *eye = camera->first_node("eye");
	float ecx = (float) atof(eye->first_node("x")->value());
	float ecy = (float) atof(eye->first_node("y")->value());
	float ecz = (float) atof(eye->first_node("z")->value());
	Vector3 eye_camera = Vector3(ecx,ecy,ecz);

	xml_node<> *target = camera->first_node("target");
	float tcx = (float) atof(target->first_node("x")->value());
	float tcy = (float) atof(target->first_node("y")->value());
	float tcz = (float) atof(target->first_node("z")->value());
	Vector3 target_camera = Vector3(tcx,tcy,tcz);

	xml_node<> *up = camera->first_node("up");
	float ucx = (float) atof(up->first_node("x")->value());
	float ucy = (float) atof(up->first_node("y")->value());
	float ucz = (float) atof(up->first_node("z")->value());
	Vector3 up_camera = Vector3(ucx,ucy,ucz);

	Camera*	camera_go = new Camera();
	camera_go->lookAt(eye_camera,Vector3(0,0,0), up_camera); //position the camera and point to 0,0,0
	camera_go->setPerspective(fov_camera,(float)game->window_width/(float)game->window_height,near_camera,far_camera); //set the projection, we want to be perspective
	
	GameObjectPlayer* player = new GameObjectPlayer();
	player->lifePoints = 100;
	player->is_collider = false;
	player->mesh = g_resourceManager.getMesh("x3_interceptor");
	player->texture_diffuse = g_resourceManager.getTexture("x3_interceptor_diffuse");
	player->texture_specular = g_resourceManager.getTexture("x3_interceptor_diffuse");
	player->shader = g_resourceManager.getShader("phong_tex");
	scene1.root->addChild(player);

	scene1.camera = camera_go;
	
	//NODES

	for(xml_node<> *node = root->first_node("node") ; node ; node = node->next_sibling()){
		LevelParser::loadObject(node, scene1.root);
	}

}


void LevelParser::loadObject(xml_node<> *mesh_node, GameObject* parent){
	std::string nom = mesh_node->first_node("name")->value();
	if(nom.compare("spawner") == 0){
		Spawner* new_mesh = new Spawner();
		new_mesh->name = nom;
		new_mesh->mesh = g_resourceManager.getMesh(mesh_node->first_node("mesh")->value());
		new_mesh->shader = g_resourceManager.getShader(mesh_node->first_node("shader")->value());
		new_mesh->texture_diffuse = g_resourceManager.getTexture(mesh_node->first_node("texture-diffuse")->value());
		new_mesh->texture_specular = g_resourceManager.getTexture(mesh_node->first_node("texture-diffuse")->value());
		new_mesh->is_collider = false;
		new_mesh->lifePoints = 2000;
		xml_node<> *transform = mesh_node->first_node("transform");
		std::cout << new_mesh->name << std::endl;
		for(xml_node<> *attributes = transform->first_node() ; attributes ; attributes = attributes->next_sibling()){
		
			if(strcmp(attributes->name(),"scale") == 0){
				float sx = (float) atof(attributes->first_node("x")->value());
				float sy = (float) atof(attributes->first_node("y")->value());
				float sz = (float) atof(attributes->first_node("z")->value());
				new_mesh->model.scale(sx,sy,sz);
			}

			if(strcmp(attributes->name(),"translate") == 0){
				float mx = (float) atof(attributes->first_node("x")->value());
				float my = (float) atof(attributes->first_node("y")->value());
				float mz = (float) atof(attributes->first_node("z")->value());
				new_mesh->model.traslate(mx,my,mz);
			}

			if(strcmp(attributes->name(),"rotate") == 0){
				float angle = (float) atof(attributes->first_node("angle")->value());
				xml_node<> *axis = attributes->first_node("axis");
				float mx = (float) atof(axis->first_node("x")->value());
				float my = (float) atof(axis->first_node("y")->value());
				float mz = (float) atof(axis->first_node("z")->value());
				Vector3 rot = Vector3(mx,my,mz);
				new_mesh->model.rotate(angle,rot);
			}


		}
		parent->addChild(new_mesh);

		for(xml_node<> *node = mesh_node->first_node("node") ; node ; node = node->next_sibling()){
			LevelParser::loadObject(node, new_mesh);
		}
	}
	if(nom.compare("enemy") == 0){
		enemyController* new_mesh = new enemyController();
		new_mesh->name = nom;
		new_mesh->mesh = g_resourceManager.getMesh(mesh_node->first_node("mesh")->value());
		new_mesh->shader = g_resourceManager.getShader("bulletShader");
		new_mesh->texture_diffuse = g_resourceManager.getTexture(mesh_node->first_node("texture-diffuse")->value());
		new_mesh->texture_specular = g_resourceManager.getTexture(mesh_node->first_node("texture-diffuse")->value());
		new_mesh->is_collider = false;
		xml_node<> *transform = mesh_node->first_node("transform");
		std::cout << new_mesh->name << std::endl;
		for(xml_node<> *attributes = transform->first_node() ; attributes ; attributes = attributes->next_sibling()){
		
			if(strcmp(attributes->name(),"scale") == 0){
				float sx = (float) atof(attributes->first_node("x")->value());
				float sy = (float) atof(attributes->first_node("y")->value());
				float sz = (float) atof(attributes->first_node("z")->value());
				new_mesh->model.scale(sx,sy,sz);
			}

			if(strcmp(attributes->name(),"translate") == 0){
				float mx = (float) atof(attributes->first_node("x")->value());
				float my = (float) atof(attributes->first_node("y")->value());
				float mz = (float) atof(attributes->first_node("z")->value());
				new_mesh->model.traslate(mx,my,mz);
			}

			if(strcmp(attributes->name(),"rotate") == 0){
				float angle = (float) atof(attributes->first_node("angle")->value());
				xml_node<> *axis = attributes->first_node("axis");
				float mx = (float) atof(axis->first_node("x")->value());
				float my = (float) atof(axis->first_node("y")->value());
				float mz = (float) atof(axis->first_node("z")->value());
				Vector3 rot = Vector3(mx,my,mz);
				new_mesh->model.rotate(angle,rot);
			}


		}
		parent->addChild(new_mesh);

		for(xml_node<> *node = mesh_node->first_node("node") ; node ; node = node->next_sibling()){
			LevelParser::loadObject(node, new_mesh);
		}
	}
	
	

	
	

}