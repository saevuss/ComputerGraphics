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
float pitch = 0;
mat3 Ry;
mat3 Rx;
mat3 R;
float speed = 0.0005f;
float rotateSpeed = 0.0005f;
		

// ----------------------------------------------------------------------------
// FUNCTIONS

void Update(void);
void Draw(void);
void VertexShader(const vec3 vertices, ivec2& projPos);
void Interpolate(ivec2 a, ivec2 b, vector<ivec2>& result);
void DrawLineSDL(ivec2 a, ivec2 b, vec3 color);
void DrawPolygonEdges(const vector <vec3>& vertices);

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
	Rx = mat3(
		vec3 (1, 0, 0), 
		vec3(0, cos(pitch), -sin(pitch)),
		vec3(0, sin(pitch), cos(pitch)));
	Ry = mat3(
		vec3 (cos(yaw), 0, -sin(yaw)), 
		vec3(0, 1, 0),
		vec3(sin(yaw), 0, cos(yaw))
	);
	R = Rx*Ry;
	
	vec3 right(		R[0][0], R[0][1], R[0][2]);
	vec3 down(		R[1][0], R[1][1], R[1][2]);
	vec3 forward (	R[2][0], R[2][1], R[2][2]);	
	const Uint8* keystate = SDL_GetKeyboardState(NULL);
	if (keystate[SDL_SCANCODE_UP]) {
		// Move camera forward
		cameraPos += dt*speed * forward;
	}
	if (keystate[SDL_SCANCODE_DOWN]) {
		// Move camera backward
		cameraPos -= dt*speed * forward;
	}
	if (keystate[SDL_SCANCODE_LEFT]) {
		// Move camera to the left
		cameraPos -= dt*speed * right;
	}
	if (keystate[SDL_SCANCODE_RIGHT]) {
		// Move camera to the right
		cameraPos += dt*speed * right;
	}
	if (keystate[SDL_SCANCODE_W]) {
		//move camera up
		cameraPos -= dt*speed*down;

	}
	if (keystate[SDL_SCANCODE_S]) {
		//move camera down
		cameraPos += dt*speed*down;
	}
	if (keystate[SDL_SCANCODE_A]) {
		// rotate camera to the left
		yaw -= rotateSpeed*dt;
	}
	if (keystate[SDL_SCANCODE_D]) {
		//rotate camera to the right
		yaw += rotateSpeed*dt;
	}
	if (keystate[SDL_SCANCODE_Q]) {
		//pitch rotation
		pitch -= rotateSpeed*dt;
	}
	if (keystate[SDL_SCANCODE_E]) {
		pitch += rotateSpeed*dt;
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
		DrawPolygonEdges(vertices);
		// for(int v=0; v<3; v++){
		// 	ivec2 projPos;
		// 	ivec2 projPos2;
		// 	vec3 color(1, 1, 1);
		// 	VertexShader(vertices[v], projPos);
		// 	VertexShader(vertices[(v+1)%3], projPos2);
		// 	DrawLineSDL(projPos, projPos2, color);	
		// 	//sdlAux->putPixel(projPos.x, projPos.y, color);
		// }


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

//task 4.3
void DrawLineSDL(ivec2 a, ivec2 b, vec3 color){
	ivec2 delta = glm::abs(a - b);
	int pixels = glm::max(delta.x, delta.y) + 1;
	vector<ivec2> line(pixels);
	Interpolate(a, b, line);
	for(int i = 0; i<pixels; i++){
		sdlAux->putPixel(line[i].x, line[i].y, color);
	}
}

//task 4.4
void DrawPolygonEdges(const vector <vec3>& vertices){
	int V = vertices.size();
	//transform each vertex from 3D world position to 2D image positoin
	vector <ivec2> projectedVertices(V);
	for(int i=0; i<V; i++){
		VertexShader(vertices[i], projectedVertices[i]);
	}
	//Loop over all vertices and draw the edge from it to the next vertex
	for(int i = 0; i<V; i++){
		int j = (i+1)%V; //the next vertex
		vec3 color(1, 1, 1);
		DrawLineSDL(projectedVertices[i], projectedVertices[j], color);
	}

}