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
vec3 currentColor;
//task 6
float depthBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
// typedef struct Pixel{
//     int x;
//     int y;
//     float zinv; //inverse depth
// };

//task7
typedef struct Vertex{
    vec3 position;
	vec3 normal;
	vec2 reflectance;
};
vec3 lightPos(0, -0.5, -0.7);
vec3 lightPower = 10.1f * vec3(1, 1, 1);
vec3 indirectLightPowerPerArea = 0.5f * vec3(1, 1, 1);

typedef struct Pixel{
    int x;
    int y;
    float zinv; //inverse depth
	vec3 illumination;
};
		

// ----------------------------------------------------------------------------
// FUNCTIONS

void Update(void);
void Draw(void);
void VertexShader(const vec3 vertices, ivec2& projPos);
void Interpolate(ivec2 a, ivec2 b, vector<ivec2>& result);
void DrawLineSDL(ivec2 a, ivec2 b, vec3 color);
void DrawPolygonEdges(const vector <vec3>& vertices);
void ComputePolygonRows(const vector<ivec2>& vertexPixels, vector<ivec2>& leftPixels, vector<ivec2>& rightPixels);
void DrawPolygonRows(const vector<ivec2>& leftPixels, const vector<ivec2>& rightPixels);
void DrawPolygon(const vector<vec3>& vertices);

//task6
void VertexShader(const vec3& vertices, Pixel& projPos);
void Interpolate(Pixel a, Pixel b, vector<Pixel>& result);
void ComputePolygonRows(const vector<Pixel>& vertexPixels, vector<Pixel>& leftPixels, vector<Pixel>& rightPixels);
void DrawPolygonRows(const vector<Pixel>& leftPixels, const vector<Pixel>& rightPixels);


//task7
void PixelShader(const Pixel& p);
void VertexShader(const Vertex& v, Pixel& p);
void DrawPolygon(const vector<Vertex>& vertices);

int main(int argc, char* argv[])
{
	LoadTestModel(triangles);  // Load model
	sdlAux = new SDL2Aux(SCREEN_WIDTH, SCREEN_HEIGHT);
	t = SDL_GetTicks();	// Set start value for timer.

	// vector<ivec2> vertexPixels(3);
	// vertexPixels[0] = ivec2(10, 5);
	// vertexPixels[1] = ivec2(5, 10);
	// vertexPixels[2] = ivec2(15, 15);
	// vector<ivec2> leftPixels;
	// vector<ivec2> rightPixels;
	// ComputePolygonRows(vertexPixels, leftPixels, rightPixels);
	// for(int row=0; row<leftPixels.size(); row++){
	// 	cout << " Start: ("
	// 	<< leftPixels[row].x<<","
	// 		<<leftPixels[row].y<<")."
	// 		<<" End: ("
	// 		<<rightPixels[row].x << ","
	// 		<<rightPixels[row].y << "). "<<endl;
	// }



	while(!sdlAux->quitEvent())
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
	for(int y=0; y<SCREEN_HEIGHT; y++){
		for(int x=0; x<SCREEN_WIDTH; x++){
			depthBuffer[y][x] = 0;
		}
	}
    

	for (int i = 0; i<triangles.size(); ++i)
	{
		//loops through all the triagnles
		// vector<vec3> vertices(3);

		// vertices[0] = triangles[i].v0;
		// vertices[1] = triangles[i].v1;
		// vertices[2] = triangles[i].v2;
		
		// for(int v=0; v<3; v++){
		// 	ivec2 projPos;
		// 	ivec2 projPos2;
		// 	vec3 color(1, 1, 1);
		// 	VertexShader(vertices[v], projPos);
		// 	VertexShader(vertices[(v+1)%3], projPos2);
		// 	DrawLineSDL(projPos, projPos2, color);	
		// 	//sdlAux->putPixel(projPos.x, projPos.y, color);
		// }

		//DrawPolygonEdges(vertices);
		//currentColor = triangles[i].color;
		//DrawPolygon(vertices);

		//task7
		vector<Vertex> vertices(3);

		vertices[0].position = triangles[i].v0;
		vertices[1].position = triangles[i].v1;
		vertices[2].position = triangles[i].v2;

		vertices[0].normal = triangles[i].normal;
		vertices[1].normal = vertices[0].normal;
		vertices[2].normal = vertices[0].normal;
		
		vertices[0].reflectance = vec2(0.8f, 0.2f);
		vertices[1].reflectance = vec2(0.8f, 0.2f);
		vertices[2].reflectance = vec2(0.8f, 0.2f);

		currentColor = triangles[i].color;
		DrawPolygon(vertices);
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

void ComputePolygonRows(const vector<ivec2>& vertexPixels, vector<ivec2>& leftPixels, vector<ivec2>& rightPixels){
	//1. find max and min y-value of the polygon and compute the number of rows it occupies
	int max_y = -251;
	int min_y = 251;
	int index_max;
	int index_min;
	for(int i=0; i<3; i++){
		if(vertexPixels[i].y <= min_y){
			min_y = vertexPixels[i].y;
			index_min = i;
		}
		if(vertexPixels[i].y >= max_y){
			max_y = vertexPixels[i].y;
			index_max = i;
		}
	}

	//2. resize left pixel and right pixel so that they have an element for each row
	int ROWS = max_y - min_y + 1;
	leftPixels.resize(ROWS);
	rightPixels.resize(ROWS);

	//3. initialize the x-coordinates in leftPixels to some really large value and the x-coordinates in rightPixels to some really small values
	for( int i =0; i<ROWS; ++i )
	{
		leftPixels[i].x += numeric_limits <int >::max();
		rightPixels[i].x = -(numeric_limits <int >::max());
	}

	//4. Loop through all edges of the polygon and use linear interpolation to find the x-coordinate for each row it occupies
	//Update the corresponding values in rightPixels and LeftPixels
	ivec2 delta1 = glm::abs(vertexPixels[index_max] - vertexPixels[3-index_min-index_max]); 
	ivec2 delta2 = glm::abs(vertexPixels[index_max] - vertexPixels[index_min]); //second edge
	ivec2 delta3 = glm::abs(vertexPixels[index_min] - vertexPixels[3-index_min-index_max]); //third edge
	int pixels1 = glm::max(delta1.x, delta1.y) + 1;
	int pixels2 = glm::max(delta2.x, delta2.y) + 1;
	int pixels3 = glm::max(delta3.x, delta3.y) + 1;
	vector<ivec2> line1(pixels1);
	vector<ivec2> line2(pixels2);
	vector<ivec2> line3(pixels3);
	Interpolate(vertexPixels[index_max], vertexPixels[3-index_min-index_max], line1); //first edge
	Interpolate(vertexPixels[index_max], vertexPixels[index_min], line2); //second edge
	Interpolate(vertexPixels[index_min], vertexPixels[3-index_min-index_max], line3); //third edge

	for(int i = 0; i < pixels1; i++){
		int row = line1[i].y - min_y;
		if(row >= 0 && row < ROWS){
			if(line1[i].x < leftPixels[row].x)  {leftPixels[row] = line1[i];}
			if(line1[i].x > rightPixels[row].x) {rightPixels[row] = line1[i];}
		}
	}
	for(int i = 0; i < pixels2; i++){
		int row = line2[i].y - min_y;
		if(row >= 0 && row < ROWS){
			if(line2[i].x < leftPixels[row].x)  {leftPixels[row] = line2[i];}
			if(line2[i].x > rightPixels[row].x) {rightPixels[row] = line2[i];}
		}
	}
	for(int i = 0; i < pixels3; i++){
		int row = line3[i].y - min_y;
		if(row >= 0 && row < ROWS){
			if(line3[i].x < leftPixels[row].x)  {leftPixels[row] = line3[i];}
			if(line3[i].x > rightPixels[row].x) {rightPixels[row] = line3[i];}
		}
	}
	
}

void DrawPolygonRows(const vector<ivec2>& leftPixels, const vector<ivec2>& rightPixels){
    int ROWS = leftPixels.size();
    for(int i=0; i<ROWS; i++){ //iterating across rows
        int rowLenght = abs(leftPixels[i].x - rightPixels[i].x) + 1; //calculating rowLenght
		// for(int j=0; j < rowLenght; j++){
		// 	sdlAux->putPixel(leftPixels[i].x + j, leftPixels[i].y, currentColor);
		// 	//If by mistake we also wrote leftPixels[i].y + j, the error image is generated.
        // }
		vector<ivec2> result(rowLenght);
		Interpolate(leftPixels[i], rightPixels[i], result);
		for(int j=0; j < rowLenght; j++){
			sdlAux->putPixel(result[j].x, result[j].y, currentColor);
        }

    }
}

void DrawPolygon(const vector<vec3>& vertices){
	int V = vertices.size();
	//vector <ivec2> vertexPixels(V);
	vector <Pixel> vertexPixels(V);
	for(int i=0; i<V; i++){
		Vertex v;
		v.position = vertices[i];
		VertexShader(v, vertexPixels[i]);
	}
	// vector<ivec2> leftPixels;
	// vector<ivec2> rightPixels;
	vector<Pixel> leftPixels;
	vector<Pixel> rightPixels;
	ComputePolygonRows(vertexPixels, leftPixels, rightPixels);
	DrawPolygonRows(leftPixels, rightPixels);

}

//task 6
void Interpolate(Pixel a, Pixel b, vector<Pixel>& result){

	//error1 image is generated if we use the step with integer values (int stepX, int stepY), loss of precision
	int N = result.size();
	float stepX = (b.x-a.x)/float(max(N-1, 1));
	float stepY = (b.y-a.y)/float(max(N-1, 1));
	float stepZinv = (b.zinv-a.zinv)/float(max(N-1, 1));
	float currentX = a.x;
    float currentY = a.y;
    float currentZinv = a.zinv;
	//Pixel current(a);


	//task7
	vec3 stepIll = (b.illumination-a.illumination)/float(max(N-1, 1));
	vec3 currentIll = a.illumination;
	for(int i = 0; i<N; i++){
		//error2 image is generated if it is used Pixel curren, this because if the step is for example 0.5 and the 
		// current.x is an int, adding 0.5 to 2 for example, will result in 2.5 which, casting to int, will return 2
		// result[i] = current;
		// current.x += stepX;
		// current.y += stepY;
		// current.zinv += stepZinv;

		//we maintain float values to iterate

		result[i].x = int(currentX);
        result[i].y = int(currentY);
        result[i].zinv = currentZinv;
		result[i].illumination = currentIll; //task7
        
        currentX += stepX;
        currentY += stepY;
        currentZinv += stepZinv;
		currentIll += stepIll;
	}
}

void ComputePolygonRows(const vector<Pixel>& vertexPixels, vector<Pixel>& leftPixels, vector<Pixel>& rightPixels){
	//1. find max and min y-value of the polygon and compute the number of rows it occupies
	int max_y = -251;
	int min_y = 251;
	int index_max;
	int index_min;
	for(int i=0; i<3; i++){
		if(vertexPixels[i].y <= min_y){
			min_y = vertexPixels[i].y;
			index_min = i;
		}
		if(vertexPixels[i].y >= max_y){
			max_y = vertexPixels[i].y;
			index_max = i;
		}
	}

	//2. resize left pixel and right pixel so that they have an element for each row
	int ROWS = max_y - min_y + 1;
	leftPixels.resize(ROWS);
	rightPixels.resize(ROWS);

	//3. initialize the x-coordinates in leftPixels to some really large value and the x-coordinates in rightPixels to some really small values
	for( int i =0; i<ROWS; ++i )
	{
		leftPixels[i].x = numeric_limits <int >::max();
		rightPixels[i].x = -(numeric_limits <int >::max());
		leftPixels[i].zinv = 0;
	}

	//4. Loop through all edges of the polygon and use linear interpolation to find the x-coordinate for each row it occupies
	//Update the corresponding values in rightPixels and LeftPixels

	
	ivec2 delta1 = ivec2(glm::abs(vertexPixels[index_max].x - vertexPixels[3-index_min-index_max].x), glm::abs(vertexPixels[index_max].y - vertexPixels[3-index_min-index_max].y)); 
	ivec2 delta2 = ivec2(glm::abs(vertexPixels[index_max].x - vertexPixels[index_min].x), glm::abs(vertexPixels[index_max].y - vertexPixels[index_min].y)); //second edge
	ivec2 delta3 = ivec2(glm::abs(vertexPixels[index_min].x - vertexPixels[3-index_min-index_max].x), glm::abs(vertexPixels[index_min].y - vertexPixels[3-index_min-index_max].y)); //third edge
	int pixels1 = glm::max(delta1.x, delta1.y) + 1;
	int pixels2 = glm::max(delta2.x, delta2.y) + 1;
	int pixels3 = glm::max(delta3.x, delta3.y) + 1;
	vector<Pixel> line1(pixels1);
	vector<Pixel> line2(pixels2);
	vector<Pixel> line3(pixels3);
	Interpolate(vertexPixels[index_max], vertexPixels[3-index_min-index_max], line1); //first edge
	Interpolate(vertexPixels[index_max], vertexPixels[index_min], line2); //second edge
	Interpolate(vertexPixels[index_min], vertexPixels[3-index_min-index_max], line3); //third edge

	for(int i = 0; i < pixels1; i++){
		int row = line1[i].y - min_y;
		if(row >= 0 && row < ROWS){
			if(line1[i].x < leftPixels[row].x)  {leftPixels[row] = line1[i];}
			if(line1[i].x > rightPixels[row].x) {rightPixels[row] = line1[i];}
		}
	}
	for(int i = 0; i < pixels2; i++){
		int row = line2[i].y - min_y;
		if(row >= 0 && row < ROWS){
			if(line2[i].x < leftPixels[row].x)  {leftPixels[row] = line2[i];}
			if(line2[i].x > rightPixels[row].x) {rightPixels[row] = line2[i];}
		}
	}
	for(int i = 0; i < pixels3; i++){
		int row = line3[i].y - min_y;
		if(row >= 0 && row < ROWS){
			if(line3[i].x < leftPixels[row].x)  {leftPixels[row] = line3[i];}
			if(line3[i].x > rightPixels[row].x) {rightPixels[row] = line3[i];}
		}
	}
	
}

void VertexShader(const vec3& v, Pixel& p){
	//transformation vertices --> //P'[1x3] = R[3x3]P[1x3] 
	vec3 t = R*(v - cameraPos);
	p.x = f*(t.x/t.z) + SCREEN_WIDTH/2; //camera is in the center
	p.y = f*(t.y/t.z) + SCREEN_HEIGHT/2; 
	p.zinv = 1/t.z;

}

void DrawPolygonRows(const vector<Pixel>& leftPixels, const vector<Pixel>& rightPixels){
    int ROWS = leftPixels.size();
    for(int i=0; i<ROWS; i++){ //iterating across rows
        int rowLenght = abs(leftPixels[i].x - rightPixels[i].x) + 1; //calculating rowLenght
        vector<Pixel> pixelsInRow(rowLenght);
		Interpolate(leftPixels[i], rightPixels[i], pixelsInRow);
		for(int j=0; j < pixelsInRow.size(); j++){
			// int x = pixelsInRow[j].x;
    		// int y = pixelsInRow[j].y;
			// if(x<0 || x >= SCREEN_WIDTH || y<0 || y >= SCREEN_HEIGHT) continue;
			// //color the pixel at the condition that zinv>=0 so that z is small
			// if(pixelsInRow[j].zinv >= depthBuffer[y][x]){ //!! it is declared as [HEIGHT][WIDTH]
			// 	depthBuffer[y][x] = pixelsInRow[j].zinv;
			// 	sdlAux->putPixel(x, y, currentColor);
			// }
			PixelShader(pixelsInRow[j]);
            
        }

    }
}

//task7
void PixelShader(const Pixel& p){
	int x = p.x;
	int y = p.y;
	if(x<0 || x >= SCREEN_WIDTH || y<0 || y >= SCREEN_HEIGHT) return;
	if(p.zinv > depthBuffer[y][x]){
		depthBuffer[y][x] = p.zinv;
		sdlAux->putPixel(x, y, p.illumination);
	}
}


void DrawPolygon(const vector<Vertex>& vertices){
	int V = vertices.size();
	//vector <ivec2> vertexPixels(V);
	vector <Pixel> vertexPixels(V);
	for(int i=0; i<V; i++){
		VertexShader(vertices[i], vertexPixels[i]);
	}
	// vector<ivec2> leftPixels;
	// vector<ivec2> rightPixels;
	vector<Pixel> leftPixels;
	vector<Pixel> rightPixels;
	ComputePolygonRows(vertexPixels, leftPixels, rightPixels);
	DrawPolygonRows(leftPixels, rightPixels);
}

void VertexShader(const Vertex& v, Pixel& p){
	//transformation vertices --> //P'[1x3] = R[3x3]P[1x3] 
	vec3 t = R*(v.position - cameraPos);
	p.x = f*(t.x/t.z) + SCREEN_WIDTH/2; //camera is in the center
	p.y = f*(t.y/t.z) + SCREEN_HEIGHT/2; 
	p.zinv = 1/t.z;
	float r = glm::length(lightPos - v.position); //difference between the two positions
	float A = 4.0f * M_PI * r * r;
	vec3 B = lightPower / A; //power per area
	
	vec3 dirSurfaceLight = glm::normalize(lightPos - v.position); 

	//calculations of intensity
	vec3 D = B * glm::max(glm::dot(v.normal, dirSurfaceLight), 0.0f);
	p.illumination = currentColor * (D + indirectLightPowerPerArea);




}
