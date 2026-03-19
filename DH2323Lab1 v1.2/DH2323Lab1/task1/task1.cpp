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


// ---------------------------------------------------------
// FUNCTION DECLARATIONS
void Draw();
// ---------------------------------------------------------

// FUNCTION DEFINITIONS
int main(int argc, char* argv[])
{	

	sdlAux = new SDL2Aux(SCREEN_WIDTH, SCREEN_HEIGHT);
	while (!sdlAux->quitEvent()) {
		Draw();
	}
	
	sdlAux->saveBMP("screenshot.bmp");
	return 0;
}

void Draw()
{
	sdlAux->clearPixels(); //screen cleared before we draw it
	

	for (int y = 0; y<SCREEN_HEIGHT; ++y)
	{	
		for (int x = 0; x<SCREEN_WIDTH; ++x)
		{	
			
			vec3 color(1.0 - (float)(y+1)/(float)(SCREEN_HEIGHT), 0.0, (float)(y+1)/(float)(SCREEN_HEIGHT));
			sdlAux->putPixel(x, y, color); // to draw single pixel
			// /************ TASK1 */
			// if(x%2==0 && y%2==0){
			// 	vec3 color(1.0, 0.0, 0.0);
			// 	sdlAux->putPixel(x, y, color); // to draw single pixel
				
			// } else{
			// 	vec3 color(0.0, 0.0, 1.0); 
			// 	sdlAux->putPixel(x, y, color); // to draw single pixel
				
			// }

		}
	}

	sdlAux->render();
}
