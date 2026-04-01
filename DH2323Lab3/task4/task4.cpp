//DH2323 skeleton code, Lab3 (SDL2 version)
#include <iostream>
#include <glm/glm.hpp>
#include "SDL2Auxiliary/SDL2Auxiliary.h"
#include "TestModel.h"
#include <algorithm> //for max()

using namespace std;
using glm::vec3;
using glm::mat3;
using glm::ivec2;
using glm::vec2;

// ----------------------------------------------------------------------------
// GLOBAL VARIABLES

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;
SDL2Aux *sdlAux;
int t;
vector<Triangle> triangles;
float f = SCREEN_HEIGHT/2;
vec3 cameraPos(0, 0, -3.001); //top left corner
float yaw = 0; //initial rotation angle
mat3 R = mat3(
		vec3 (cos(yaw), 0, -sin(yaw)), 
		vec3(0, 1, 0),
		vec3(sin(yaw), 0, cos(yaw)));

// ----------------------------------------------------------------------------
// FUNCTIONS

void Update(void);
void Draw(void);
void VertexShader(const vec3 vertices, ivec2& projPos);
void Interpolate(ivec2 a, ivec2 b, vector<ivec2>& result);
void DrawLineSDL(ivec2 a, ivec2 b, vec3 color);

int main(int argc, char* argv[])
{
	LoadTestModel(triangles);  // Load model
	sdlAux = new SDL2Aux(SCREEN_WIDTH, SCREEN_HEIGHT);
	t = SDL_GetTicks();	// Set start value for timer.

	while (!sdlAux->quitEvent())
	{
		Update();
		Draw();
	}
	sdlAux->saveBMP("screenshot.bmp");
	return 0;
}

void Update(void)
{
	// Compute frame time:
	int t2 = SDL_GetTicks();
	float dt = float(t2 - t);
	t = t2;
	//cout << "Render time: " << dt << " ms." << endl;

	const Uint8* keystate = SDL_GetKeyboardState(NULL);
	if (keystate[SDL_SCANCODE_UP]) {
		// Move camera forward
	}
	if (keystate[SDL_SCANCODE_DOWN]) {
		// Move camera backward
	}
	if (keystate[SDL_SCANCODE_LEFT]) {
		// Move camera to the left
	}
	if (keystate[SDL_SCANCODE_RIGHT]) {
		// Move camera to the right
	}
	if (keystate[SDL_SCANCODE_W]) {

	}
	if (keystate[SDL_SCANCODE_S]) {

	}
	if (keystate[SDL_SCANCODE_A]) {

	}
	if (keystate[SDL_SCANCODE_D]) {

	}
	if (keystate[SDL_SCANCODE_Q]) {

	}
	if (keystate[SDL_SCANCODE_E]) {

	}
}

void Draw()
{
	sdlAux->clearPixels();

	for (int i = 0; i<triangles.size(); ++i)
	{
		//loops through all the triagnles
		vector<vec3> vertices(3);

		vertices[0] = triangles[i].v0;
		vertices[1] = triangles[i].v1;
		vertices[2] = triangles[i].v2;

		//loops through all the vertices of the triangles
		bool flag = true;
		for(int v=0; v<3; v++){
			ivec2 projPos;
			ivec2 projPos2;
			vec3 color(1, 1, 1);
			VertexShader(vertices[v], projPos);
			VertexShader(vertices[(v+1)%3], projPos2);
			DrawLineSDL(projPos, projPos2, color);	
			//sdlAux->putPixel(projPos.x, projPos.y, color);
		}


	}

	sdlAux->render();
}

void VertexShader(const vec3 vertices, ivec2& projPos){
	//transformation vertices --> //P'[1x3] = R[3x3]P[1x3] 
	vec3 t = R*(vertices - cameraPos);
	projPos.x = f*(t.x/t.z) + SCREEN_WIDTH/2; //camera is in the center
	projPos.y = f*(t.y/t.z) + SCREEN_HEIGHT/2; 

}

void Interpolate(ivec2 a, ivec2 b, vector<ivec2>& result){
	int N = result.size();
	vec2 step = vec2(b-a)/float(max(N-1, 1));
	vec2 current(a);
	for(int i = 0; i<N; i++){
		result[i] = current;
		current+= step;
	}
}

//task 4.4
void DrawLineSDL(ivec2 a, ivec2 b, vec3 color){
	ivec2 delta = glm::abs(a - b);
	int pixels = glm::max(delta.x, delta.y) + 1;
	vector<ivec2> line(pixels);
	Interpolate(a, b, line);
	for(int i = 0; i<pixels; i++){
		sdlAux->putPixel(line[i].x, line[i].y, color);
	}
}