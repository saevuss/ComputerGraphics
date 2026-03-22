// DH2323 Lab 1
// Introduction lab that covers:
// * SDL2 (https://www.libsdl.org/)
// * C++, std::vector and glm::vec3 (https://glm.g-truc.net)
// * 2D graphics
// * Plotting pixels
// * Video memory
// * Color representation
// * Linear interpolation

#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "SDL2Auxiliary/SDL2Auxiliary.h"

using namespace std;
using glm::vec3;

// ---------------------------------------------------------
// GLOBAL VARIABLES
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
SDL2Aux *sdlAux;

/****** TASK3 */
vector<vec3> stars( 1000 );
int t;


// ---------------------------------------------------------
// FUNCTION DECLARATIONS
void Draw();
void Update(); //TASK3.4
// ---------------------------------------------------------

// FUNCTION DEFINITIONS
int main(int argc, char* argv[])
{	

	/*************** TASK3 *************/
	for(int s = 0; s< stars.size(); s++){ //initializing the location
		stars[s].x = 2 * float(rand()) / float(RAND_MAX) -1 ; //-1 ≤ x ≤ 1
		stars[s].y = 2 *float(rand()) / float(RAND_MAX) - 1 ; //-1 ≤ y ≤ 1
		stars[s].z = float(rand()) / float(RAND_MAX); //0 ≤ z ≤ 1
	}

	t = SDL_GetTicks();
	/*********************************** */

	sdlAux = new SDL2Aux(SCREEN_WIDTH, SCREEN_HEIGHT);
	while (!sdlAux->quitEvent()) {
		Update();
		Draw();
	}
	
	sdlAux->saveBMP("screenshot.bmp");
	return 0;
}

void Draw()
{
	sdlAux->clearPixels(); //screen cleared before we draw it
	
	//coloring with black
	for (int y = 0; y<SCREEN_HEIGHT; ++y)
	{			
		for (int x = 0; x<SCREEN_WIDTH; ++x)
		{	
			vec3 color(0.0,0.0,0.0);
			sdlAux->putPixel(x, y, color);
		}
	}

	int focal_lenght = SCREEN_HEIGHT/2;
	
	for(size_t i = 0; i<stars.size(); i++){
		vec3 color2 = 0.2f * vec3(1.0,1.0,1.0) / (stars[i].z* stars[i].z);
		int u = focal_lenght*(stars[i].x/stars[i].z) + SCREEN_WIDTH/2;
		int v = focal_lenght*(stars[i].y/stars[i].z) + SCREEN_HEIGHT/2;
		sdlAux->putPixel(u, v, color2);
	}


	for (int y = 0; y<SCREEN_HEIGHT; ++y)
	{	
		for (int x = 0; x<SCREEN_WIDTH; ++x)
		{	

			sdlAux->putPixel(x, y, color); // to draw single pixel

		}
	}

	sdlAux->render();
}

/** TASK 3.4 */
void Update(){
	int t2 = SDL_GetTicks();
	float dt = float(t2-t); //holds the number of milliseconds that has passed since update was called last time
	t = t2; //update to the new value of the time
	float speed = 0.0005f;
	for(int s = 0; s< stars.size(); s++){
		stars[s].z = stars[s].z - speed*dt;
		if( stars[s].z <= 0 ) stars[s].z += 1;
		if( stars[s].z > 1 ) stars[s].z -= 1;
	}
}
