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
void Interpolate(float a, float b, vector<float>& result); //TASK2	
void InterpolateColorVec(vec3 colo1, vec3 color2, vector<vec3>& resultColor);
void Update(); //TASK3.4
// ---------------------------------------------------------

// FUNCTION DEFINITIONS
int main(int argc, char* argv[])
{	
	//************ TASK2 */
	vector<float> result(10);
	Interpolate(5, 14, result);
	for(int i=0; i<result.size(); i++){
		cout << result[i] << " ";
	}
	vector<vec3> resultVec(4);
	vec3 color1(1,4,9.2);
	vec3 color2(4,1,9.8);
	InterpolateColorVec(color1, color2, resultVec);
	for(int j=0; j < resultVec.size(); j++){
		cout << "( "
		<< resultVec[j].x << ", "
		<< resultVec[j].y << ", "
		<< resultVec[j].z << ")  ";
	}

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
	
	/*********** TASK2 ***************/
	vec3 topLeft(1,0,0); //red
	vec3 topRight(0,0,1); //blue
	vec3 bottomLeft(0,1,0); //green
	vec3 bottomRight(1,1,0); //yellow
	vector<vec3> leftSide( SCREEN_HEIGHT );
	vector<vec3> rightSide( SCREEN_HEIGHT );
	InterpolateColorVec( topLeft, bottomLeft, leftSide );
	InterpolateColorVec( topRight, bottomRight, rightSide );


	for (int y = 0; y<SCREEN_HEIGHT; ++y)
	{	
		/*********** TASK2 ***************/
		vector<vec3> row( SCREEN_WIDTH );
		InterpolateColorVec(leftSide[y], rightSide[y], row);
		/**************************/
		for (int x = 0; x<SCREEN_WIDTH; ++x)
		{	
			//************ TASK2 */
			sdlAux->putPixel(x, y, row[x]);	


		}
	}

	sdlAux->render();
}

/** TASK 2.1 */
void Interpolate(float a, float b, vector<float>& result){
	//in this way it takes into account the result size trace of the 
	for(int i = a; i<b+1 && (i-a)<result.size() ; i++){
		result[i-a] = i;
	}
}

/** TASK 2.2 */
void InterpolateColorVec(vec3 color1, vec3 color2, vector<vec3>& resultColor){
	int n = resultColor.size();
	for(int i = 0; i<n; i++){
		// t goes from 0.0 (first element = color1) to 1.0 (last element = color2)
		float t = (n == 1) ? 0.0f: float(i)/float(n-1); //t represent the percentage in which i am, so if i'm in the middle of the screen, t will be 0.5
		resultColor[i] = color1 + t*(color2 - color1); //standard linear interpolation form
	}

}

