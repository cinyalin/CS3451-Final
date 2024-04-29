//#####################################################################
// Main
// Dartmouth COSC 77/177 Computer Graphics, starter code
// Contact: Bo Zhu (bo.zhu@dartmouth.edu)
//#####################################################################
#include <iostream>

#include <random>
#include "Common.h"
#include "Particles.h"
#include "OpenGLMesh.h"
#include "OpenGLCommon.h"
#include "OpenGLWindow.h"
#include "OpenGLViewer.h"
#include "OpenGLMarkerObjects.h"
#include "OpenGLParticles.h"

/////////////////////////////////////////////////////////////////////
//// TODO: put your name in the string               
/////////////////////////////////////////////////////////////////////

const std::string author="glsl tutorial";

const std::string draw_pixels = To_String(

const float PIXEL_SIZE=1.f;

//////////////////////////////////////////////////////////////////////////
//// demo 0: draw a circle
vec4 paint_circle(float x,float y) {
	vec2 center = vec2(iResolution / PIXEL_SIZE / 2.); // window center
	float radius = 100.f/PIXEL_SIZE;
	if(pow(x-center.x,2)+pow(y-center.y,2)<radius*radius)
		return vec4(1.f,1.f,0.f,1.f);
	else return vec4(0.f,0.f,0.f,1.f);
}

//////////////////////////////////////////////////////////////////////////
//// demo 1: draw a triangle
vec2 center = vec2(iResolution / PIXEL_SIZE / 2.); // window center
vec2 v0 = center - vec2(100.,0.);
vec2 v1 = center + vec2(100.,0.);
vec2 v2 = center + vec2(0.,173.);
vec2 e0 = v1 - v0;
vec2 e1 = v2 - v1;
vec2 e2 = v0 - v2;

mat2 rot = mat2(0.,-1.,+1.,0.);		//// glsl matrix is column major
vec2 n0 = rot*e0;
vec2 n1 = rot*e1;
vec2 n2 = rot*e2;

vec4 paint_triangle(float x,float y) {
	vec2 p = vec2(x,y);
	
	if(dot(p-v0,n0)<=0. && dot(p-v1,n1)<=0. && dot(p-v2,n2)<=0.){
		return vec4(1.,1.,0.,1.);
	}
	else return vec4(0.,0.,0.,1.);
}

//////////////////////////////////////////////////////////////////////////
//// demo 2: draw a checkerboard
float mod(float x,float y){
	return x-y*floor(x/y);
}

vec4 paint_checkerboard(float x, float y) {
	if(mod(x,32.)<16.&&mod(y,32.)>=16.||
		mod(x,32.)>=16.&&mod(y,32.)<16.)
		return vec4(0,0,0,1.);
	else 
		return vec4(1,1,1,1.);
}

//////////////////////////////////////////////////////////////////////////
//// demo 3: draw a sin wave
vec4 paintSin(float x,float y) {
	vec2 center = vec2(iResolution / PIXEL_SIZE / 2.); // window center
	float x0=x-center.x;
	float y0=y-center.y;
	float f=100.*sin(x0/50.);
	if(abs(f-y0)<2.)
		return vec4(.5*(1.+cos(x0/20.)),1.,.5*(1.+sin(x0/20.)),1.);
	else 
		return vec4(0.,0.,0.,1.);
}

// Return the rgba color of the grid at position (x, y) 
vec4 paintGrid(float x, float y) {
	//// uncomment the following functions to see different demo implementations
	return paint_circle(x,y);
	return paint_triangle(x,y);
	//return paint_checkerboard(x,y);
	//return paintSin(x,y);
}

// The function called in the fragment shader
void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
	// To divide the screen into the grids for painting!
	fragColor = paintGrid(floor(fragCoord.x / PIXEL_SIZE), floor(fragCoord.y / PIXEL_SIZE));
}

);

class A0_Driver : public OpenGLViewer
{
	OpenGLScreenCover* screen_cover = nullptr;
	clock_t startTime = clock();

public:
	virtual void Initialize()
	{
		opengl_window=std::make_shared<OpenGLWindow>();
		opengl_window->win_w=640;
		opengl_window->win_h=640;		
		
		OpenGLViewer::Initialize();
	}

	//// Initialize the screen covering mesh and shaders
	virtual void Initialize_Data()
	{
		OpenGLShaderLibrary::Instance()->Create_Screen_Shader(draw_pixels, "shaderToy");
		screen_cover = Add_Interactive_Object<OpenGLScreenCover>();
		Set_Polygon_Mode(screen_cover, PolygonMode::Fill);
		Uniform_Update();

		screen_cover->Set_Data_Refreshed();
		screen_cover->Initialize();
		screen_cover->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("shaderToy"));
	}

	//// Update the uniformed variables used in shader
	void Uniform_Update()
	{
		screen_cover->setResolution((float)Win_Width(), (float)Win_Height());
		screen_cover->setTime(GLfloat(clock() - startTime) / CLOCKS_PER_SEC);
	}

	//// Go to next frame 
	virtual void Toggle_Next_Frame()
	{
		Uniform_Update();
		OpenGLViewer::Toggle_Next_Frame();
	}

	////Keyboard interaction
	virtual void Initialize_Common_Callback_Keys()
	{
		OpenGLViewer::Initialize_Common_Callback_Keys();
	}

	virtual void Run()
	{
		OpenGLViewer::Run();
	}
};

int main(int argc,char* argv[])
{
	if(author==""){std::cerr<<"***** The author name is not specified. Please put your name in the author string first. *****"<<std::endl;return 0;}
	else std::cout<<"Assignment 0 demo by "<<author<<" started"<<std::endl;

	A0_Driver driver;
	driver.Initialize();
	driver.Run();	
}

