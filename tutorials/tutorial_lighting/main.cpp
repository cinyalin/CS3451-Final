//#####################################################################
// Main
// Dartmouth COSC 77/177 Computer Graphics, starter code
// Contact: Bo Zhu (bo.zhu@dartmouth.edu)
//#####################################################################
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include "Common.h"
#include "OpenGLMesh.h"
#include "OpenGLCommon.h"
#include "OpenGLWindow.h"
#include "OpenGLViewer.h"
#include "TinyObjLoader.h"

#ifndef __Main_cpp__
#define __Main_cpp__

#ifdef __APPLE__
#define CLOCKS_PER_SEC 100000
#endif

class ShaderDriver : public OpenGLViewer
{
	std::vector<OpenGLTriangleMesh*> mesh_object_array;						////mesh objects, every object you put in this array will be rendered.
	clock_t startTime;

public:
	virtual void Initialize()
	{
		////For those who are unhappy about the current background color: Here is a secret passage. 
		////Goto OpenGLShaderProgrammcpp Line 239 and 240, change the two colors, and you will get a different background.
		draw_bk=true;						////this flag specifies a customized way to draw the background. If you turn it off, there is no background.
		draw_axes=false;					////if you don't like the axes, turn them off!
		startTime = clock();
		OpenGLViewer::Initialize();
	}

	////This function adds a mesh object from an obj file
	int Add_Obj_Mesh_Object(std::string obj_file_name)
	{
		auto mesh_obj=Add_Interactive_Object<OpenGLTriangleMesh>();

		Array<std::shared_ptr<TriangleMesh<3> > > meshes;
		Obj::Read_From_Obj_File(obj_file_name,meshes);
		mesh_obj->mesh=*meshes[0];
		std::cout<<"load tri_mesh from obj file, #vtx: "<<mesh_obj->mesh.Vertices().size()<<", #ele: "<<mesh_obj->mesh.Elements().size()<<std::endl;		

		mesh_object_array.push_back(mesh_obj);
		return (int)mesh_object_array.size()-1;
	}

	////This function adds a sphere mesh
	int Add_Sphere_Object(const double radius=1.)
	{
		auto mesh_obj=Add_Interactive_Object<OpenGLTriangleMesh>();

		Initialize_Sphere_Mesh(radius,&mesh_obj->mesh,3);		////add a sphere with radius=1. if the obj file name is not specified

		mesh_object_array.push_back(mesh_obj);
		return (int)mesh_object_array.size()-1;
	}

	////This function adds a triangle (the in-class demo)
	int Add_Triangle_Object(const std::vector<Vector3>& vertices)
	{
		auto mesh_obj=Add_Interactive_Object<OpenGLTriangleMesh>();
		auto& mesh=mesh_obj->mesh;

		////manually initialize the vertices and elements for a triangle mesh
		mesh.Vertices().resize(3);
		for(int i=0;i<vertices.size();i++)mesh.Vertices()[i]=vertices[i];
		mesh.Elements().resize(1);mesh.Elements()[0]=Vector3i(0,1,2);

		mesh_object_array.push_back(mesh_obj);
		return (int)mesh_object_array.size()-1;
	}

	////This function demonstrates how to manipulate the vertex array of a mesh on the CPU end.
	////The updated vertices will be sent to GPU for rendering automatically.
	void Translate_Vertex_Position_For_Mesh_Object(OpenGLTriangleMesh* obj,const Vector3& translate)
	{
		std::vector<Vector3>& vertices=obj->mesh.Vertices();		
		for(auto& v:vertices){
			v+=translate;
		}
	}

	////This function demonstrates how to manipulate the color and normal arrays of a mesh on the CPU end.
	////The updated colors and normals will be sent to GPU for rendering automatically.
	void Update_Vertex_Color_And_Normal_For_Mesh_Object(OpenGLTriangleMesh* obj)
	{
		int vn=(int)obj->mesh.Vertices().size();					////number of vertices of a mesh
		std::vector<Vector3>& vertices=obj->mesh.Vertices();		////you might find this array useful
		std::vector<Vector3i>& elements=obj->mesh.Elements();		////you might find this array also useful

		std::vector<Vector4f>& vtx_color=obj->vtx_color;
		vtx_color.resize(vn);
		std::fill(vtx_color.begin(),vtx_color.end(),Vector4f::Zero());

		for(int i=0;i<vn;i++){
			vtx_color[i]=Vector4f(1.,1.,0.,1.);	////specify color for each vertex
		}

		std::vector<Vector3>& vtx_normal=obj->vtx_normal;
		vtx_normal.resize(vn);
		std::fill(vtx_normal.begin(),vtx_normal.end(),Vector3::Zero());

		for(int i=0;i<vn;i++){
			vtx_normal[i]=Vector3(vertices[i][0],vertices[i][1],vertices[i][2]);
		}
	}

	virtual void Initialize_Data()
	{
		////Add two sphere meshes
		{
			int obj_idx=Add_Sphere_Object();
			auto obj=mesh_object_array[obj_idx];
			Update_Vertex_Color_And_Normal_For_Mesh_Object(obj);		
		}

		{
			int obj_idx=Add_Sphere_Object(.1);
			auto obj=mesh_object_array[obj_idx];
			Update_Vertex_Color_And_Normal_For_Mesh_Object(obj);
			Translate_Vertex_Position_For_Mesh_Object(obj,Vector3(2.,0.,0.));
		}

		////initialize shader
		std::string vertex_shader_file_name="helloworld.vert";
		std::string fragment_shader_file_name="helloworld.frag";
		OpenGLShaderLibrary::Instance()->Add_Shader_From_File(vertex_shader_file_name,fragment_shader_file_name,"a2_shader");

		////bind the shader with each mesh object in the object array
		for(auto& mesh_obj: mesh_object_array){
			mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("a2_shader"));
			Set_Polygon_Mode(mesh_obj,PolygonMode::Fill);
			Set_Shading_Mode(mesh_obj,ShadingMode::A2);
			mesh_obj->Set_Data_Refreshed();
			mesh_obj->Initialize();	
		}
	}

	//// Go to next frame 
	virtual void Toggle_Next_Frame()
	{
		for (auto& mesh_obj : mesh_object_array) {
			mesh_obj->setTime(GLfloat(clock() - startTime) / CLOCKS_PER_SEC);
		}
		OpenGLViewer::Toggle_Next_Frame();
	}

	virtual void Run()
	{
		OpenGLViewer::Run();
	}
};

int main(int argc,char* argv[])
{
	ShaderDriver driver;
	driver.Initialize();
	driver.Run();	
}

#endif