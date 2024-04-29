#version 330 core

/*default camera matrices. do not modify.*/
layout(std140) uniform camera {
    mat4 projection;		/*camera's projection matrix*/
    mat4 view;			/*camera's view matrix*/
    mat4 pvm;			/*camera's projection*view*model matrix*/
    mat4 ortho;			/*camera's ortho projection matrix*/
    vec4 position;		/*camera's position in world space*/
};


layout(location = 0) in vec4 pos;

out vec3 vtx_model_position;

void main() 
{
    vtx_model_position = pos.xyz; // pos acts as 3D uvw coordinates
	// ! get rid of the translation part of the view matrix; set z to w; see https://learnopengl.com/Advanced-OpenGL/Cubemaps
    gl_Position = (projection * mat4(mat3(view)) * vec4(pos.xyz, 1.0)).xyww;
}