//#####################################################################
// Main
// Dartmouth COSC 77/177 Computer Graphics, starter code
// Contact: Bo Zhu (bo.zhu@dartmouth.edu)
//#####################################################################
#include <iostream>
#include <random>
#include <unordered_set>
#include "Common.h"
#include "Particles.h"
#include "OpenGLMesh.h"
#include "OpenGLCommon.h"
#include "OpenGLWindow.h"
#include "OpenGLViewer.h"
#include "OpenGLMarkerObjects.h"
#include "OpenGLParticles.h"
#include "TinyObjLoader.h"

#ifndef __Main_cpp__
#define __Main_cpp__

class MeshDriver : public OpenGLViewer
{
	OpenGLTriangleMesh* opengl_tri_mesh=nullptr;						////mesh
	TriangleMesh<3>* tri_mesh=nullptr;
	OpenGLSegmentMesh* opengl_normals=nullptr;							////normals
	OpenGLSegmentMesh* opengl_edges=nullptr;							////edges

	bool use_obj_mesh=false;											////flag for use obj, set it to be true if you want to load an obj mesh

public:
	virtual void Initialize()
	{
		OpenGLViewer::Initialize();
	}

	void Load_Mesh()
	{
		//Initialize_Icosahedron_Mesh(.5,tri_mesh);
		//Create_Circle_Mesh(Vector3::Zero(),1.,32,*tri_mesh);
		Create_Cube_Mesh(1.,*tri_mesh);
		Find_Vertex_Neighbors(7,*tri_mesh);	
	}

	virtual void Initialize_Data()
	{
		////initialize tri mesh
		opengl_tri_mesh=Add_Interactive_Object<OpenGLTriangleMesh>();
		tri_mesh=&opengl_tri_mesh->mesh;

		Load_Mesh();

		opengl_edges=Add_Interactive_Object<OpenGLSegmentMesh>();
		Set_Polygon_Mode(opengl_edges,PolygonMode::Fill);
		Set_Shading_Mode(opengl_edges,ShadingMode::None);
		Update_OpenGL_Seg_Mesh();

		Set_Polygon_Mode(opengl_tri_mesh,PolygonMode::Fill);
		Set_Shading_Mode(opengl_tri_mesh,ShadingMode::None);
		Set_Color(opengl_tri_mesh,OpenGLColor(.3f,.3f,.3f,1.f));
		opengl_tri_mesh->Set_Data_Refreshed();
		opengl_tri_mesh->Initialize();
	}

	////synchronize data to visualization
	void Sync_Simulation_And_Visualization_Data()
	{
		opengl_tri_mesh->Set_Data_Refreshed();
		opengl_tri_mesh->Initialize();
		Update_OpenGL_Seg_Mesh();
	}

	////update simulation and visualization for each time step
	virtual void Toggle_Next_Frame()
	{
		Sync_Simulation_And_Visualization_Data();
		OpenGLViewer::Toggle_Next_Frame();
	}

	virtual void Run()
	{
		OpenGLViewer::Run();
	}

	//// demo
	void Use_Eigen_Vectors()
	{
		using namespace std;

		Vector2 v=Vector2(1.,2.);
		Vector2 v2=Vector2(2.,3.);
		Vector2 v3=v+v2;
		Vector3i v4=Vector3i(1,2,3);
		float dot_prod=v.dot(v2);
		float norm=v.norm();
		Vector2 normal=v.normalized();
		v.normalize();

		std::cout<<"v: "<<v.transpose()<<std::endl;
		std::cout<<v[0]<<", "<<v[1]<<std::endl;
	}

	void Create_Circle_Mesh(const Vector3& center,double radius,int n,TriangleMesh<3>& tri_mesh)
	{
		tri_mesh.Clear();

		//std::vector<Vector3>& vtx=tri_mesh.Vertices();
		//std::vector<Vector3i>& tri=tri_mesh.Elements();

		//double two_pi=2.*3.1415927;
		//double theta=two_pi/(double)n;

		//vtx.push_back(center+Vector3(0,0,0.5));
		//for(int i=0;i<n;i++){
		//	Vector3 p=center+Vector3(radius*cos(theta*(double)i),radius*sin(theta*(double)i),(double)0);
		//	vtx.push_back(p);
		//}

		//for(int i=1;i<=n;i++){
		//	tri.push_back(Vector3i(0,i,(i+1>n?(i+1-n):(i+1))));
		//}

		//std::cout<<"tri elements: "<<tri_mesh.elements.size()<<std::endl;
		//for(int i=0;i<tri_mesh.elements.size();i++){
		//	std::cout<<tri_mesh.elements[i].transpose()<<std::endl;
		//}

		std::vector<Vector3>& vtx=tri_mesh.Vertices();
		std::vector<Vector3i>& tri=tri_mesh.Elements();

		vtx.resize(n+1);
		tri.resize(n);

		double two_pi=2*3.1415927;
		double angle=two_pi/(double)n;

		vtx[0]=center;
		for(int i=1;i<n+1;i++){
			double theta=angle*i;
			vtx[i]=Vector3(radius*cos(theta),radius*sin(theta),0.);
		}

		vtx.push_back(Vector3(1.f,0.f,0.f));

		vtx[0]=center+Vector3(0,0,1);

		for(int i=0;i<n-1;i++){
			tri.push_back(Vector3i(0,i+1,i+2));
		}
		tri.push_back(Vector3i(0,n,1));

		//for(int i=0;i<n;i++){
		//	tri.push_back(Vector3i(0,(i+1),(i+2)%n));
		//}
	}

	void Create_Cube_Mesh(double length,TriangleMesh<3>& tri_mesh)
	{
		std::vector<Vector3>& vtx=tri_mesh.Vertices();
		std::vector<Vector3i>& tri=tri_mesh.Elements();

		vtx={Vector3(0,0,0),Vector3(1,0,0),Vector3(0,1,0),Vector3(1,1,0),
			 Vector3(0,0,1),Vector3(1,0,1),Vector3(0,1,1),Vector3(1,1,1)};
		for(int i=0;i<vtx.size();i++){
			vtx[i]*=length;
		}

		tri={Vector3i(4,5,7),Vector3i(4,7,6),
			Vector3i(5,1,7),Vector3i(7,1,3),
			Vector3i(2,3,1),Vector3i(0,2,1),
			Vector3i(6,2,4),Vector3i(2,0,4),
			Vector3i(2,6,3),Vector3i(6,7,3),
			Vector3i(0,1,4),Vector3i(1,5,4)};
	}

	void Find_Vertex_Neighbors(const int p,TriangleMesh<3>& tri_mesh)
	{
		std::unordered_map<int,std::vector<int> > vtx_vtx_map;

		std::vector<Vector3>& vtx=tri_mesh.Vertices();
		std::vector<Vector3i>& tri=tri_mesh.Elements();

		for(int i=0;i<tri.size();i++){
			Vector3i& t=tri[i];
			for(int j=0;j<3;j++){
				if(vtx_vtx_map.find(t[j])==vtx_vtx_map.end()){
					vtx_vtx_map[t[j]]=std::vector<int>();
				}
				std::vector<int>& nbs=vtx_vtx_map[t[j]];
				for(int k=0;k<3;k++){
					if(t[k]==t[j])continue;
					if(std::find(nbs.begin(),nbs.end(),t[k])!=nbs.end())continue;
					nbs.push_back(t[k]);
				}
			}
		}

		auto& p_nbs=vtx_vtx_map[p];
		std::cout<<"nbs of "<<p<<": "<<std::endl;
		for(auto& v:p_nbs){std::cout<<v<<", ";}std::cout<<std::endl;
	}

protected:
	void Get_Triangle_Mesh_Edges(const TriangleMesh<3>& triangle_mesh,std::unordered_set<Vector2i>& hashset)
	{
		for(int i=0;i<triangle_mesh.elements.size();i++){
			const Vector3i& tri=triangle_mesh.elements[i];
			for(int j=0;j<3;j++){
				Vector2i edge(tri[j],tri[(j+1)%3]);
				if(edge[0]>edge[1]){int tmp=edge[0];edge[0]=edge[1];edge[1]=tmp;}
				hashset.insert(edge);}}
	}

	void Update_OpenGL_Seg_Mesh()
	{
		auto seg_mesh=&opengl_edges->mesh;
		seg_mesh->elements.clear();
		std::unordered_set<Vector2i> edge_hashset;
		Get_Triangle_Mesh_Edges(*tri_mesh,edge_hashset);
		(*seg_mesh->vertices)=(*tri_mesh->vertices);
		for(auto& e:edge_hashset)
			seg_mesh->elements.push_back(e);
		opengl_edges->Set_Data_Refreshed();
		opengl_edges->Initialize();
		Set_Line_Width(opengl_edges,3.f);
		Set_Shading_Mode(opengl_edges,ShadingMode::None);
		Set_Color(opengl_edges,OpenGLColor(.1f,1.f,.1f,1.f));
	}
};

int main(int argc,char* argv[])
{
	MeshDriver driver;
	//driver.Use_Eigen_Vectors();		
	driver.Initialize();
	driver.Run();	
}

#endif