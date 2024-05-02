#include "Common.h"
#include "OpenGLCommon.h"
#include "OpenGLMarkerObjects.h"
#include "OpenGLBgEffect.h"
#include "OpenGLMesh.h"
#include "OpenGLViewer.h"
#include "OpenGLWindow.h"
#include "TinyObjLoader.h"
#include "OpenGLSkybox.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <unordered_set>
#include <vector>
#include <string>

#ifndef __Main_cpp__
#define __Main_cpp__

#ifdef __APPLE__
#define CLOCKS_PER_SEC 100000
#endif

class MyDriver : public OpenGLViewer
{
    std::vector<OpenGLTriangleMesh *> mesh_object_array;
    OpenGLBgEffect *bgEffect = nullptr;
    OpenGLSkybox *skybox = nullptr;
    clock_t startTime;

public:
    virtual void Initialize()
    {
        draw_axes = false;
        startTime = clock();
        OpenGLViewer::Initialize();
    }

    virtual void Initialize_Data()
    {
        //// Load all the shaders you need for the scene 
        //// In the function call of Add_Shader_From_File(), we specify three names: 
        //// (1) vertex shader file name
        //// (2) fragment shader file name
        //// (3) shader name used in the shader library
        //// When we bind a shader to an object, we implement it as follows:
        //// object->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("shader_name"));
        //// Here "shader_name" needs to be one of the shader names you created previously with Add_Shader_From_File()

        OpenGLShaderLibrary::Instance()->Add_Shader_From_File("shaders/basic.vert", "shaders/basic.frag", "basic");
        OpenGLShaderLibrary::Instance()->Add_Shader_From_File("shaders/basic.vert", "shaders/environment.frag", "environment");
        OpenGLShaderLibrary::Instance()->Add_Shader_From_File("shaders/stars.vert", "shaders/stars.frag", "stars");
        OpenGLShaderLibrary::Instance()->Add_Shader_From_File("shaders/basic.vert", "shaders/alphablend.frag", "blend");
        OpenGLShaderLibrary::Instance()->Add_Shader_From_File("shaders/billboard.vert", "shaders/alphablend.frag", "billboard");
        OpenGLShaderLibrary::Instance()->Add_Shader_From_File("shaders/terrain.vert", "shaders/terrain.frag", "terrain");
        OpenGLShaderLibrary::Instance()->Add_Shader_From_File("shaders/skybox.vert", "shaders/skybox.frag", "skybox");
        OpenGLShaderLibrary::Instance()->Add_Shader_From_File("shaders/mountain.vert", "shaders/mountain.frag", "mountain");

        //// Load all the textures you need for the scene
        //// In the function call of Add_Shader_From_File(), we specify two names:
        //// (1) the texture's file name
        //// (2) the texture used in the texture library
        //// When we bind a texture to an object, we implement it as follows:
        //// object->Add_Texture("tex_sampler", OpenGLTextureLibrary::Get_Texture("tex_name"));
        //// Here "tex_sampler" is the name of the texture sampler2D you used in your shader, and
        //// "tex_name" needs to be one of the texture names you created previously with Add_Texture_From_File()

        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/earth_color.png", "sphere_color");
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/earth_normal.png", "sphere_normal");
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/bunny_color.jpg", "bunny_color");
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/bunny_normal.png", "bunny_normal");
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/window.png", "window_color");
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/buzz_color.png", "buzz_color");
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/sunset_texture.jpg", "sunset_color");
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/sunset.png", "sunset2_color");
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/star.png", "star_color");
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/GiantWaterLilies_BaseColor.png", "lilies_color");
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/lotus_petal.jpg", "lotus_petal");
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/cloud_normal.png", "cloud_color");
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/cloud2_normal.png", "cloud2_color");
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/cloud3_normal.png", "cloud3_color");

        //// Add all the lights you need for the scene (no more than 4 lights)
        //// The four parameters are position, ambient, diffuse, and specular.
        //// The lights you declared here will be synchronized to all shaders in uniform lights.
        //// You may access these lights using lt[0].pos, lt[1].amb, lt[1].dif, etc.
        //// You can also create your own lights by directly declaring them in a shader without using Add_Light().
        //// Here we declared three default lights for you. Feel free to add/delete/change them at your will.

        opengl_window->Add_Light(Vector3f(3, 1, 3), Vector3f(0.1, 0.1, 0.1), Vector3f(1, 1, 1), Vector3f(0.5, 0.5, 0.5)); 
        opengl_window->Add_Light(Vector3f(0, 0, -5), Vector3f(0.1, 0.1, 0.1), Vector3f(0.9, 0.9, 0.9), Vector3f(0.5, 0.5, 0.5));
        opengl_window->Add_Light(Vector3f(-5, 1, 3), Vector3f(0.1, 0.1, 0.1), Vector3f(0.9, 0.9, 0.9), Vector3f(0.5, 0.5, 0.5));

        //// Add the background / environment
        //// Here we provide you with four default options to create the background of your scene:
        //// (1) Gradient color (like A1 and A2; if you want a simple background, use this one)
        //// (2) Programmable Canvas (like A7 and A8; if you consider implementing noise or particles for the background, use this one)
        //// (3) Sky box (cubemap; if you want to load six background images for a skybox, use this one)
        //// (4) Sky sphere (if you want to implement a sky sphere, enlarge the size of the sphere to make it colver the entire scene and update its shaders for texture colors)
        //// By default, Option (2) (Buzz stars) is turned on, and all the other three are commented out.
        
        //// Background Option (1): Gradient color
        
        // {
        //     auto bg = Add_Interactive_Object<OpenGLBackground>();
        //     bg->Set_Color(OpenGLColor(.7f, .8f, 1.f, 1.f), OpenGLColor(.5f, .7f, .9f, 1.f));
        //     bg->Initialize();
        // }
        
        /*
        //// Background Option (2): Programmable Canvas
        //// By default, we load a GT buzz + a number of stars
        
        {
            bgEffect = Add_Interactive_Object<OpenGLBgEffect>();
            bgEffect->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("stars"));
            bgEffect->Add_Texture("sunset", OpenGLTextureLibrary::Get_Texture("sunset2_color")); // bgEffect can also Add_Texture
            bgEffect->Initialize();
        }
        */
        
        
        //// Background Option (3): Sky box
        //// Here we provide a default implementation of a sky box; customize it for your own sky box
        
        {
            // from https://www.humus.name/index.php?page=Textures
            const std::vector<std::string> cubemap_files{
                "cubemap/water_posx.jpg",     //// + X
                "cubemap/water_negx.jpg",     //// - X
                "cubemap/water_posy.jpg",     //// + Y
                "cubemap/water_negy.jpg",     //// - Y
                "cubemap/water_posz.jpg",     //// + Z
                "cubemap/water_negz.jpg",     //// - Z 
            };
            OpenGLTextureLibrary::Instance()->Add_CubeMap_From_Files(cubemap_files, "cube_map");

            skybox = Add_Interactive_Object<OpenGLSkybox>();
            skybox->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("skybox"));
            skybox->Initialize();
        }
        

        /*
        //// Background Option (4): Sky sphere
        //// Here we provide a default implementation of a textured sphere; customize it for your own sky sphere
        {
            //// create object by reading an obj mesh
            auto sphere = Add_Obj_Mesh_Object("obj/sphere.obj");

            //// set object's transform
            Matrix4f t;
            t << 1, 0, 0, -1.5,
                0, 1, 0, -1,
                0, 0, 1, 0.5,
                0, 0, 0, 1;
            sphere->Set_Model_Matrix(t);

            //// set object's material
            sphere->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            sphere->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            sphere->Set_Ks(Vector3f(2, 2, 2));
            sphere->Set_Shininess(128);

            //// bind texture to object
            sphere->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("sphere_color"));
            sphere->Add_Texture("tex_normal", OpenGLTextureLibrary::Get_Texture("sphere_normal"));

            //// bind shader to object
            sphere->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        }
        */

        /*
        //// Here we load a bunny object with the basic shader to show how to add an object into the scene
        {
            //// create object by reading an obj mesh
            auto bunny = Add_Obj_Mesh_Object("obj/bunny.obj");

            //// set object's transform
            Matrix4f t;
            t << 1, 0, 0, 1.5,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1;
            bunny->Set_Model_Matrix(t);

            //// set object's material
            bunny->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            bunny->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            bunny->Set_Ks(Vector3f(2, 2, 2));
            bunny->Set_Shininess(128);

            //// bind texture to object
            bunny->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("bunny_color"));
            bunny->Add_Texture("tex_normal", OpenGLTextureLibrary::Get_Texture("bunny_normal"));

            //// bind shader to object
            bunny->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        }
        */


        // billboard implementation
        {
            //// create object by reading an obj mesh
            auto cloud = Add_Obj_Mesh_Object("obj/sqad.obj");

            //// set object's transform
            Matrix4f t;
            t << 5., 0, 0, 0.,
                0, 2., 0, 1.5,
                0, 0, 5., 2.,
                0, 0, 0, 1.;
            cloud->Set_Model_Matrix(t);

            //// bind texture to object
            cloud->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("cloud_color"));

            //// bind shader to object
            cloud->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("billboard"));
        }

        // cloud2 placement
        {
            //// create object by reading an obj mesh
            auto cloud = Add_Obj_Mesh_Object("obj/sqad.obj");

            //// set object's transform
            Matrix4f t;
            t << 3., 0, 0, -5.,
                0, 2.5, 0, 1.5,
                0, 0, 3., -1.,
                0, 0, 0, 1.;
            cloud->Set_Model_Matrix(t);

            //// bind texture to object
            cloud->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("cloud2_color"));

            //// bind shader to object
            cloud->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("billboard"));
        }
        // cloud3 placement
        {
            //// create object by reading an obj mesh
            auto cloud = Add_Obj_Mesh_Object("obj/sqad.obj");

            //// set object's transform
            Matrix4f t;
            t << 2., 0, 0, 5.,
                0, 2., 0, 1.5,
                0, 0, 2., 1.,
                0, 0, 0, 1.;
            cloud->Set_Model_Matrix(t);

            //// bind texture to object
            cloud->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("cloud3_color"));

            //// bind shader to object
            cloud->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("billboard"));
        }
        // cloud3 placement
        {
            //// create object by reading an obj mesh
            auto cloud = Add_Obj_Mesh_Object("obj/sqad.obj");

            //// set object's transform
            Matrix4f t;
            t << 5., 0, 0, -2.,
                0, 5., 0, 1.5,
                0, 0, 5., 2.,
                0, 0, 0, 1.;
            cloud->Set_Model_Matrix(t);

            //// bind texture to object
            cloud->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("cloud3_color"));

            //// bind shader to object
            cloud->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("billboard"));
        }

        // 1st set of lily pads
        {
            //// create object by reading an obj mesh
            auto lily = Add_Obj_Mesh_Object("obj/GiantWaterLilies.obj");

            //// set object's transform
            Matrix4f t;
            t << .05, 0, 0, 0.,
                0, .05, 0, -1.,
                0, 0, .05, 3.,
                0, 0, 0, 1;
            lily->Set_Model_Matrix(t);

            //// set object's material
            lily->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            lily->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            lily->Set_Ks(Vector3f(2, 2, 2));
            lily->Set_Shininess(128);

            //// bind texture to object
            lily->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("lilies_color"));

            //// bind shader to object
            lily->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        }
        
        // 2nd set of lily pads
        {
            //// create object by reading an obj mesh
            auto lily = Add_Obj_Mesh_Object("obj/GiantWaterLilies.obj");

            //// set object's transform
            Matrix4f t;
            t << .05, 0, 0, -1.,
                0, .05, 0, -1.,
                0, 0, .05, -2.,
                0, 0, 0, 1.;
            float angle = 3.1415927f * -.25f; // rotate 45 degrees counter-clockwisely 
			Matrix4f r;
			r << cos(angle), 0., -sin(angle), 0.,
				0., 1., 0., 0.,
				sin(angle), 0., cos(angle), 0.,
				0., 0., 0., 1.;
            lily->Set_Model_Matrix(t * r);

            //// set object's material
            lily->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            lily->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            lily->Set_Ks(Vector3f(2, 2, 2));
            lily->Set_Shininess(128);

            //// bind texture to object
            lily->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("lilies_color"));

            //// bind shader to object
            lily->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        }
        // 3rd set of lily pads
        {
            //// create object by reading an obj mesh
            auto lily = Add_Obj_Mesh_Object("obj/GiantWaterLilies.obj");

            //// set object's transform
            Matrix4f t;
            t << .07, 0, 0, 3.,
                0, .07, 0, -1.,
                0, 0, .07, 1.,
                0, 0, 0, 1.;
            float angle = 3.1415927f * -.5f; // 90 degrees counter-clockwisely 
			Matrix4f r;
			r << cos(angle), 0., -sin(angle), 0.,
				0., 1., 0., 0.,
				sin(angle), 0., cos(angle), 0.,
				0., 0., 0., 1.;
            lily->Set_Model_Matrix(t * r);

            //// set object's material
            lily->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            lily->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            lily->Set_Ks(Vector3f(2, 2, 2));
            lily->Set_Shininess(128);

            //// bind texture to object
            lily->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("lilies_color"));

            //// bind shader to object
            lily->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        }

        // lotus placement 
        int lotus_num = 6;
        for (int i = 0; i < lotus_num; i++) {
            auto lotus = Add_Obj_Mesh_Object("obj/lotus.obj");
            lotus->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("lotus_petal"));
            {
                // 360 / 6 = 60
                Matrix4f t;
				t << 0.09, 0., 0., i * cos(60 * i * 3.14f / 180),
					0., 0.09, 0.,  -1.08,
					0., 0., 0.09, i * sin(60 * i * 3.14f / 180),
					0., 0., 0., 1.;
				lotus->Set_Model_Matrix(t);
            }
            lotus->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        }
        // lotus2 placement
        int lotus2_num = 5;
        for (int i = 0; i < lotus2_num; i++) {
            auto lotus = Add_Obj_Mesh_Object("obj/lotus2.obj");
            lotus->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("lotus_petal"));
            {
                // 360 / 5 = 72
                Matrix4f t;
				t << 0.1, 0., 0., i * cos(72 * i * 3.14f / 180) + 1.,
					0., 0.1, 0.,  -1.08,
					0., 0., 0.1, i * sin(72 * i * 3.14f / 180) - 1.,
					0., 0., 0., 1.;
				lotus->Set_Model_Matrix(t);
            }
            lotus->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        }

        //// Here we show an example of adding a mesh with noise-terrain (A6)
        {
            //// create object by reading an obj mesh
            auto terrain = Add_Obj_Mesh_Object("obj/plane.obj");

            //// set object's transform
            float angle = 3.1415927f * .5f;           
            Matrix4f r, t;
            r << 1, 0, 0, 0,
                0, cos(angle), -sin(angle), 0,
                0, sin(angle), cos(angle), 0,
                0, 0, 0, 1;
            t << 4, 0, 0, -10,
                 0, 1, 0, -1,
                 0, 0, 5, -10,
                 0, 0, 0, 1,
            terrain->Set_Model_Matrix(t * r);

            //// set object's material
            terrain->Set_Ka(Vector3f(0.1f, 0.1f, 0.1f));
            terrain->Set_Kd(Vector3f(0.7f, 0.7f, 0.7f));
            terrain->Set_Ks(Vector3f(1, 1, 1));
            terrain->Set_Shininess(128.f);

            //// bind shader to object (we do not bind texture for this object because we create noise for texture)
            terrain->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("terrain"));
        }

        //// Here we show an example of adding a mesh with noise-terrain (A6)
        {
            //// create object by reading an obj mesh
            auto mountains = Add_Obj_Mesh_Object("obj/plane.obj");

            //// set object's transform
            float angle = 3.1415927f * .5f;
            Matrix4f r, t;
            r << 1, 0, 0, 0,
                0, cos(angle), -sin(angle), 0,
                0, sin(angle), cos(angle), 0,
                0, 0, 0, 1;
            t << 4, 0, 0, -10,
                 0, 1, 0, -1,
                 0, 0, 5, -30,
                 0, 0, 0, 1,
            mountains->Set_Model_Matrix(t * r);

            //// set object's material
            mountains->Set_Ka(Vector3f(0.1f, 0.1f, 0.1f));
            mountains->Set_Kd(Vector3f(0.6f, 0.6f, 0.6f));
            mountains->Set_Ks(Vector3f(1, 1, 1));
            mountains->Set_Shininess(128.f);

            //// bind shader to object (we do not bind texture for this object because we create noise for texture)
            mountains->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("mountain"));
        }

        /*
        //// Here we show an example of adding a transparent object with alpha blending
        //// This example will be useful if you implement objects such as tree leaves, grass blades, flower pedals, etc.
        //// Alpha blending will be turned on automatically if your texture has the alpha channel
        {
            //// create object by reading an obj mesh
            auto sqad = Add_Obj_Mesh_Object("obj/sqad.obj");

            //// set object's transform
            Matrix4f t;
            t << 1, 0, 0, -0.5,
                0, 1, 0, 0,
                0, 0, 1, 1.5,
                0, 0, 0, 1;
            sqad->Set_Model_Matrix(t);

            //// bind texture to object
            sqad->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("window_color"));

            //// bind shader to object
            sqad->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("blend"));
        }

        //// Here we show an example of adding a billboard particle with a star shape using alpha blending
        //// The billboard is rendered with its texture and is always facing the camera.
        //// This example will be useful if you plan to implement a CPU-based particle system.
        {
            //// create object by reading an obj mesh
            auto sqad = Add_Obj_Mesh_Object("obj/sqad.obj");

            //// set object's transform
            Matrix4f t;
            t << 1, 0, 0, 0,
                 0, 1, 0, 0,
                 0, 0, 1, 2.5,
                 0, 0, 0, 1;
            sqad->Set_Model_Matrix(t);

            //// bind texture to object
            sqad->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("star_color"));

            //// bind shader to object
            sqad->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("billboard"));
        }

        //// Here we show an example of shading (ray-tracing) a sphere with environment mapping
        
        {
            //// create object by reading an obj mesh
            auto sphere2 = Add_Obj_Mesh_Object("obj/sphere.obj");

            //// set object's transform
            Matrix4f t;
            t << .6, 0, 0, 0,
                0, .6, 0, -.5,
                0, 0, .6, 1,
                0, 0, 0, 1;
            sphere2->Set_Model_Matrix(t);

            //// bind shader to object
            sphere2->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("environment")); // bind shader to object
        }
        */

        // firefly implementation
        {
            bgEffect = Add_Interactive_Object<OpenGLBgEffect>();
            bgEffect->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("stars"));
            bgEffect->Initialize();
        }

        //// This for-loop updates the rendering model for each object on the list
        for (auto &mesh_obj : mesh_object_array){
            Set_Polygon_Mode(mesh_obj, PolygonMode::Fill);
            Set_Shading_Mode(mesh_obj, ShadingMode::TexAlpha);
            mesh_obj->Set_Data_Refreshed();
            mesh_obj->Initialize();
        }
        Toggle_Play();
    }

    OpenGLTriangleMesh *Add_Obj_Mesh_Object(std::string obj_file_name)
    {
        auto mesh_obj = Add_Interactive_Object<OpenGLTriangleMesh>();
        Array<std::shared_ptr<TriangleMesh<3>>> meshes;
        // Obj::Read_From_Obj_File(obj_file_name, meshes);
        Obj::Read_From_Obj_File_Discrete_Triangles(obj_file_name, meshes);

        mesh_obj->mesh = *meshes[0];
        std::cout << "load tri_mesh from obj file, #vtx: " << mesh_obj->mesh.Vertices().size() << ", #ele: " << mesh_obj->mesh.Elements().size() << std::endl;

        mesh_object_array.push_back(mesh_obj);
        return mesh_obj;
    }

    //// Go to next frame
    virtual void Toggle_Next_Frame()
    {
        for (auto &mesh_obj : mesh_object_array)
            mesh_obj->setTime(GLfloat(clock() - startTime) / CLOCKS_PER_SEC);

        if (bgEffect){
            bgEffect->setResolution((float)Win_Width(), (float)Win_Height());
            bgEffect->setTime(GLfloat(clock() - startTime) / CLOCKS_PER_SEC);
            bgEffect->setFrame(frame++);
        }
        OpenGLViewer::Toggle_Next_Frame();
    }

    virtual void Run()
    {
        OpenGLViewer::Run();
    }
};

int main(int argc, char *argv[])
{
    MyDriver driver;
    driver.Initialize();
    driver.Run();
}

#endif