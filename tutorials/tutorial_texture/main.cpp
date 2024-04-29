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
		//Obj::Read_From_Obj_File(obj_file_name,meshes);						//// load a default obj mesh
		Obj::Read_From_Obj_File_Discrete_Triangles(obj_file_name,meshes);	//// load an obj mesh with discrete triangles

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
	int Add_Square_Object(const std::vector<Vector3>& vertices)
	{
		auto mesh_obj=Add_Interactive_Object<OpenGLTriangleMesh>();
		auto& mesh=mesh_obj->mesh;

		////manually initialize the vertices and elements for a triangle mesh
		mesh.Vertices().resize(4);
		for(int i=0;i<vertices.size();i++)mesh.Vertices()[i]=vertices[i];

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
			vtx_color[i]=Vector4f(1.,1.,1.,1.);	////specify color for each vertex
		}

		std::vector<Vector3>& vtx_normal=obj->vtx_normal;
		vtx_normal.resize(vn);
		std::fill(vtx_normal.begin(),vtx_normal.end(),Vector3::Zero());

		//TODO: update the normal for each vertex
		//NOTICE: This code updates the vertex normal array on the CPU end. The array will then be sent to GPU and read it the vertex shader as normal.
		//This is a default implementation of vertex normal that works for a sphere centered around the origin only.
		for(int i=0;i<vn;i++){
			vtx_normal[i]=Vector3(vertices[i][0],vertices[i][1],vertices[i][2]);
		}	
	}

	void Update_Vertex_UV_For_Mesh_Object(OpenGLTriangleMesh* obj)
	{
		int vn=(int)obj->mesh.Vertices().size();					////number of vertices of a mesh
		std::vector<Vector3>& vertices=obj->mesh.Vertices();		////you might find this array useful
		std::vector<Vector2>& uv=obj->mesh.Uvs();					////you need to set values in uv to specify the texture coordinates
		uv.resize(vn);


		Update_Uv_Using_Spherical_Coordinates(vertices,uv);
	}

	////TODO [Step 0]: update the uv coordinates for each vertex.
	////NOTICE: This code updates the vertex color array on the CPU end. The array will then be sent to GPU and read it the vertex shader as v_color.
	////You don't need to implement the CPU-GPU data transfer code.
	void Update_Uv_Using_Spherical_Coordinates(const std::vector<Vector3>& vertices,std::vector<Vector2>& uv)
	{
		int vn=(int)vertices.size();
		for(int i=0;i<vn;i++){
			const Vector3 pos=vertices[i];
			real u = atan(pos[0], pos[2]) / (2.0*3.14159) + 0.5;
			real v = 0.5 - asin(pos[1]) / 3.14159;
			uv[i]=Vector2(u,v);			
		}	
	}

	virtual void Initialize_Data()
	{
		////Add a sphere mesh
		//{
		//	int obj_idx=Add_Sphere_Object();
		//	auto obj=mesh_object_array[obj_idx];
		//	Update_Vertex_Color_And_Normal_For_Mesh_Object(obj);		
		//	Update_Vertex_UV_For_Mesh_Object(obj);
		//}




		////Add an obj mesh
		////TODO [Step 4]: uncomment this part and use your own mesh for Step 4.
		//{
		//	 int obj_idx=Add_Obj_Mesh_Object("bunny.obj");
		//	 auto obj=mesh_object_array[obj_idx];
		//	 Update_Vertex_Color_And_Normal_For_Mesh_Object(obj);
		//}

		// //Add a manually built triangle mesh (with a single triangle). This is the demo code I showed in class.
		// //You don't need this part for your homework. Just put them here for your reference.
		//{
		//	std::vector<Vector3> triangle_vertices={Vector3(0,0,0),Vector3(1,0,0),Vector3(0,1,0),Vector3(1,1,0)};
		//	int obj_idx=Add_Square_Object(triangle_vertices);	////add a square
		//	auto obj=mesh_object_array[obj_idx];
		//	
		//	//specify the vertex colors on the CPU end
		//	std::vector<Vector4f>& vtx_color=obj->vtx_color;
		//	vtx_color={Vector4f(1.f,0.f,0.f,1.f),Vector4f(0.f,1.f,0.f,1.f),Vector4f(0.f,0.f,1.f,1.f),Vector4f(1.f,1.f,0.f,1.f)};

		//	std::vector<Vector3>& vtx_normal=obj->vtx_normal;
		//	vtx_normal={Vector3(0.,0.,1.),Vector3(0.,0.,1.),Vector3(0.,0.,1.),Vector3(0.,0.,1.)};

		//	std::vector<Vector2>& uv=obj->mesh.Uvs();
		//	uv={Vector2(0.,0.),Vector2(1.,0.),Vector2(0.,1.),Vector2(1.,1.)};

		//	std::vector<Vector3i>& elements=obj->mesh.Elements();
		//	elements={Vector3i(0,1,3),Vector3i(0,3,2)};
		//}

		////load a mesh
		{
			int obj_idx=Add_Obj_Mesh_Object("plane.obj");
			auto obj=mesh_object_array[obj_idx];	
			Update_Vertex_Color_And_Normal_For_Mesh_Object(obj);
			
			int vn=(int)obj->mesh.Vertices().size();					////number of vertices of a mesh
			std::vector<Vector3>& vertices=obj->mesh.Vertices();		////you might find this array useful
			std::vector<Vector2>& uv=obj->mesh.Uvs();					////you need to set values in uv to specify the texture coordinates
			uv.resize(vn);
			for(int i=0;i<vn;i++){
				uv[i]=Vector2(obj->mesh.Vertices()[i][0],obj->mesh.Vertices()[i][1]);}
		}

		////initialize shader
		std::string vertex_shader_file_name="hello_world.vert";
		std::string fragment_shader_file_name="hello_world.frag";
		OpenGLShaderLibrary::Instance()->Add_Shader_From_File(vertex_shader_file_name,fragment_shader_file_name,"a3_shader");

		////specifying the textures
		OpenGLTextureLibrary::Instance()->Add_Texture_From_File("dart_albedo.jpg", "albedo");
		OpenGLTextureLibrary::Instance()->Add_Texture_From_File("earth_normal.png", "normal");

		////bind the shader with each mesh object in the object array
		for(auto& mesh_obj: mesh_object_array){
			mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("a3_shader"));
			mesh_obj->Add_Texture("tex_albedo", OpenGLTextureLibrary::Get_Texture("albedo"));
			mesh_obj->Add_Texture("tex_normal", OpenGLTextureLibrary::Get_Texture("normal"));
			Set_Polygon_Mode(mesh_obj,PolygonMode::Fill);
			Set_Shading_Mode(mesh_obj,ShadingMode::Texture);
			mesh_obj->Set_Data_Refreshed();
			mesh_obj->Initialize();}
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