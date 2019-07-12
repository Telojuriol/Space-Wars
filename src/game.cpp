#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "rendertotexture.h"
#include "shader.h"
#include "managers.h"
#include "GameObjectEnvironment.h"
#include "GameObjectMesh.h"
#include <cmath>
#include "scenes.h"
#include "LevelParser.h"

//some globals
Mesh* patata = NULL;
Mesh* prova = NULL;
Mesh* sphere = NULL;
Texture* texture = NULL;
Texture* specularTexture = NULL;
Shader* shader = NULL;
float angle = 0;
RenderToTexture* rt = NULL;
GameObjectEnvironment* environment;
GameObjectMesh* nau;
std::vector<GameObjectMesh> naus;
Game* Game::instance = NULL;

std::vector<GameObject*> colliders;


Mesh* proveps;

Game::Game(SDL_Window* window)
{
	this->window = window;
	instance = this;
	// initialize attributes
	// Warning: DO NOT CREATE STUFF HERE, USE THE INIT 
	// things create here cannot access opengl
	SDL_GetWindowSize( window, &window_width, &window_height );
	std::cout << " * Window size: " << window_width << " x " << window_height << std::endl;

	keystate = NULL;
	mouse_locked = false;
}

//Here we have already GL working, so we can create meshes and textures
void Game::init(void)
{
    std::cout << " * Path: " << getPath() << std::endl;
	char *f = "data/assets/level1.xml";
	
	wayPoints.resize(50);

	int max = 2000;
	int min = -2000;

	for(int i = 0; i < wayPoints.size(); i++){
		int randNumX = (rand()%(max-min + 1) + min);
		int randNumY = (rand()%(max-min + 1) + min);
		int randNumZ = (rand()%(max-800 + 1) + 800);
		wayPoints[i].set(randNumX,randNumY,randNumZ);


	}


    //SDL_SetWindowSize(window, 50,50);
	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	proveps = new Mesh();
	proveps->createPlane(40);

	g_resourceManager.loadMesh("bullet", "data/assets/bullet.ASE");
	g_resourceManager.loadShader("bulletShader", "data/shaders/fire.vert","data/shaders/fire.frag");
	LevelParser::loadLevel("data/assets/level1.xml");


	g_bulletManager = new BulletManager(g_resourceManager.getMesh("bullet"),1000);
	g_bulletManager->shader = g_resourceManager.getShader("bulletShader");
	/*


	if( !shader->load("data/shaders/simple.vert","data/shaders/simple.frag") )
	{
		std::cout << "shader not found or error" << std::endl;
		//if your program quits straight after opening, the shader probably hasn't compiled,
		//if you put a breakpoint here you'll be able to read the compilation error in the console
		exit(0);
	}*/

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}

//what to do when the image has to be draw
void Game::render(void)
{
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//update view and projection matrices, and push gl matrices for fixed pipeline rendering
	scene1.camera->set();
	
	
	scene1.render();
	g_bulletManager->render();
	//disable blending to draw text on top
    glDisable( GL_BLEND );

	//*** Drawing code ends here ***

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
	double speed = seconds_elapsed * 100; //the speed is defined by the seconds_elapsed so it goes constant

	
	
	scene1.update(seconds_elapsed);
	colliders.clear();
	scene1.getColliders(colliders, scene1.root);
	g_bulletManager->checkCollisions(colliders);

	for(int i = 0; i < colliders.size(); i++){

		for(int j = 0; j < colliders.size(); j++){

			//testCollision

		}

	}
	
	
	/*

	if ((mouse_state & SDL_BUTTON_LEFT) || mouse_locked ) //is left button pressed?
	{
		scene1.camera->rotate(mouse_delta.x * 0.005f, Vector3(0,-1,0));
		scene1.camera->rotate(mouse_delta.y * 0.005f, scene1.camera->getLocalVector( Vector3(-1,0,0)));
	}
	if(keystate[SDL_SCANCODE_LSHIFT]) speed *= 10; //move faster with left shift
	if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) scene1.camera->move(Vector3(0,0,1) * (float)speed);
	if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) scene1.camera->move(Vector3(0,0,-1) * (float)speed);
	if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT]) scene1.camera->move(Vector3(1,0,0) * (float)speed);
	if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT]) scene1.camera->move(Vector3(-1,0,0) * (float)speed);

	if (mouse_locked)
	{
		int center_x = (int)floor(window_width*0.5f);
		int center_y = (int)floor(window_height*0.5f);
        //center_x = center_y = 50;
		SDL_WarpMouseInWindow(this->window, center_x, center_y); //put the mouse back in the middle of the screen
		//SDL_WarpMouseGlobal(center_x, center_y); //put the mouse back in the middle of the screen
        
        this->mouse_position.x = (float)center_x;
        this->mouse_position.y = (float)center_y;
	}
    */
	//to navigate with the mouse fixed in the middle
	
}

//Keyboard event handler (sync input)
void Game::onKeyPressed( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: exit(0); //ESC key, kill the app
	}
}


void Game::onMouseButton( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
}

void Game::setWindowSize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
    
	/*
    Uint32 flags = SDL_GetWindowFlags(window);
    if(flags & SDL_WINDOW_ALLOW_HIGHDPI)
    {
        width *= 2;
        height *= 2;
    }
	*/

	glViewport( 0,0, width, height );
	scene1.camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}




/*
    <node>
      <name>enemy</name>
      <mesh>x3_fighter</mesh>
      <shader>phong_tex</shader>
      <texture-diffuse>x3_fighter_diffuse</texture-diffuse>
      <transform>
        <translate>
          <x>-700</x>
          <y>300</y>
          <z>1000</z>
        </translate>
      </transform>
    </node>
    <node>
      <name>enemy</name>
      <mesh>x3_fighter</mesh>
      <shader>phong_tex</shader>
      <texture-diffuse>x3_fighter_diffuse</texture-diffuse>
      <transform>
        <translate>
          <x>700</x>
          <y>300</y>
          <z>1000</z>
        </translate>
      </transform>
    </node>
    <node>
      <name>enemy</name>
      <mesh>x3_fighter</mesh>
      <shader>phong_tex</shader>
      <texture-diffuse>x3_fighter_diffuse</texture-diffuse>
      <transform>
        <translate>
          <x>1200</x>
          <y>500</y>
          <z>2000</z>
        </translate>
      </transform>
    </node>
    <node>
      <name>enemy</name>
      <mesh>x3_fighter</mesh>
      <shader>phong_tex</shader>
      <texture-diffuse>x3_fighter_diffuse</texture-diffuse>
      <transform>
        <translate>
          <x>1200</x>
          <y>500</y>
          <z>1500</z>
        </translate>
      </transform>
    </node>
    <node>
      <name>enemy</name>
      <mesh>x3_fighter</mesh>
      <shader>phong_tex</shader>
      <texture-diffuse>x3_fighter_diffuse</texture-diffuse>
      <transform>
        <translate>
          <x>-1200</x>
          <y>-500</y>
          <z>700</z>
        </translate>
      </transform>
    </node>
    <node>
      <name>enemy</name>
      <mesh>x3_fighter</mesh>
      <shader>phong_tex</shader>
      <texture-diffuse>x3_fighter_diffuse</texture-diffuse>
      <transform>
        <translate>
          <x>-1200</x>
          <y>500</y>
          <z>700</z>
        </translate>
      </transform>
    </node>
    <node>
      <name>enemy</name>
      <mesh>x3_fighter</mesh>
      <shader>phong_tex</shader>
      <texture-diffuse>x3_fighter_diffuse</texture-diffuse>
      <transform>
        <translate>
          <x>1200</x>
          <y>-500</y>
          <z>700</z>
        </translate>
      </transform>
    </node>
    <node>
      <name>enemy</name>
      <mesh>x3_fighter</mesh>
      <shader>phong_tex</shader>
      <texture-diffuse>x3_fighter_diffuse</texture-diffuse>
      <transform>
        <translate>
          <x>1200</x>
          <y>500</y>
          <z>700</z>
        </translate>
      </transform>
    </node>
    <node>
      <name>enemy</name>
      <mesh>x3_fighter</mesh>
      <shader>phong_tex</shader>
      <texture-diffuse>x3_fighter_diffuse</texture-diffuse>
      <transform>
        <translate>
          <x>0</x>
          <y>0</y>
          <z>2000</z>
        </translate>
      </transform>
    </node>




*/