//DH2323 skeleton code, Lab2 (SDL2 version)
#include <iostream>
#include <glm/glm.hpp>
#include "../SDL2Auxiliary/SDL2Auxiliary.h"
#include "TestModel.h"
#include <algorithm>

using namespace std;
using glm::vec3;
using glm::mat3;

// ----------------------------------------------------------------------------
// GLOBAL VARIABLES

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;
SDL2Aux *sdlAux;
int t;
vector<Triangle> triangles;
float m = std::numeric_limits<float>::max();
typedef struct Intersection{
	vec3 position;
	float distance;
	int triangleIndex;
};
Intersection closestIntersection;

// ----------------------------------------------------------------------------
// FUNCTIONS

void Update(void);
void Draw(void);

bool ClosestIntersection(vec3 start, vec3 dir, const vector<Triangle>& triangles, Intersection& closestIntersection);


int main( int argc, char* argv[] )
{
	sdlAux = new SDL2Aux(SCREEN_WIDTH, SCREEN_HEIGHT);
	t = SDL_GetTicks();	// Set start value for timer.

	/*TASK 2.1*/
	LoadTestModel(triangles); 
	// for(int i=0; i<triangles.size(); i++){
	// 	std::cout << "v0: " << triangles[i].v0.x << ", " << triangles[i].v0.y << ", " << triangles[i].v0.z << "\n";
	// }

	while (!sdlAux->quitEvent())
	{
		//Update();
		Draw();
	}
	sdlAux->saveBMP("screenshot.bmp");
	return 0;
}

void Update(void)
{
	// Compute frame time:
	int t2 = SDL_GetTicks();
	float dt = float(t2-t);
	t = t2;
	cout << "Render time: " << dt << " ms." << endl;
}

void Draw()
{
	sdlAux->clearPixels();

	// for( int y=0; y<SCREEN_HEIGHT; ++y )
	// {
	// 	for( int x=0; x<SCREEN_WIDTH; ++x )
	// 	{
	// 		vec3 color( 1, 0.5, 0.5 );
	// 		sdlAux->putPixel(x, y, color);
	// 	}
	// }

	vec3 cameraPos(0, 0, -2); //because x and y are form -1 to 1, the camera is centered and outside the scene
    float focalLength = SCREEN_HEIGHT/2; //initializating the focal lenght as the half of the screen_height as in lab1
	for(int y=0; y<SCREEN_HEIGHT; y++){
        for(int x=0; x<SCREEN_WIDTH; x++){
			//calculate the direction of the ray for the single pixel (x, y)
			// nb: the center of the screen is in (250, 250)
            vec3 dir(
                x - SCREEN_WIDTH/2, 
                y - SCREEN_HEIGHT/2,
                focalLength
            );
            Intersection intersection;
			//calculating if the ray through the pixel is intersecting one of the triangles
            if(ClosestIntersection(cameraPos, dir, triangles, intersection)){
                sdlAux->putPixel(x, y, triangles[intersection.triangleIndex].color); //uses the color of the closest intersection (closest to the camera, so it covers whatever is behind)
            } else {
                sdlAux->putPixel(x, y, vec3(0,0,0));
            }
        }
    }

	sdlAux->render();
}

bool ClosestIntersection(vec3 start, vec3 dir, const vector<Triangle>& triangles, Intersection& closestIntersection){
	float min_r = m;
	for(int tr=0; tr<triangles.size(); tr++){
		//iterate through all the triangles
		vec3 v0 = triangles[tr].v0;
		vec3 v1 = triangles[tr].v1;
		vec3 v2 = triangles[tr].v2;
		vec3 e1 = v1 - v0;
		vec3 e2 = v2 - v0;
		vec3 b = start - v0;
		mat3 A(-dir, e1, e2);
		float detA = det(A);
		if(detA==0){
			continue;
		}
		float t = det(mat3(b, e1, e2))/detA;
		float u = det(mat3(-dir, b, e2))/detA;
		float v = det(mat3(-dir, e1, b))/detA;
		
		vec3 x = vec3(t, u, v);
		//vec3 x = glm::inverse(A) * b; // x = t, u, v = x.x, x.y, x.z
		
		if(x.y >= 0 && x.z >= 0 && x.y + x.z <= 1 && x.x>=0){
			//valid intersection if u and v are >=0 and for those point whose are actually within the triangle
			vec3 r = start + x.x*dir; 
			if(x.x <= min_r){
				//update of the closest intersection 
				closestIntersection.distance = x.x;
				closestIntersection.triangleIndex = tr;
				closestIntersection.position = r;
				min_r = x.x;
			}
		}

	}
	return min_r != m;
}

float det(mat3 A){
	//for a 3x3 matrix, the determinant is given by a sum of products along diagonals 
	float sumUpDownDiagonals = A[0][0]*A[1][1]*A[2][2] + A[0][1]*A[1][2]*A[2][0] + A[0][2]*A[1][0]*A[2][1];
	float sumDownUpDiagonals = A[2][0]*A[1][1]*A[0][2] + A[2][1]*A[1][2]*A[0][0] + A[2][2]*A[1][0]*A[0][1];
	return sumUpDownDiagonals - sumDownUpDiagonals;
}


