/*This is your first fragment shader!*/

#version 330 core

/*default camera matrices. do not modify.*/
layout (std140) uniform camera
{
	mat4 projection;	/*camera's projection matrix*/
	mat4 view;			/*camera's view matrix*/
	mat4 pvm;			/*camera's projection*view*model matrix*/
	mat4 ortho;			/*camera's ortho projection matrix*/
	vec4 position;		/*camera's position in world space*/
};

// Passed time from the begining of the program
uniform float iTime;
// uniform variables of albedo textures and normal textures
uniform sampler2D tex_albedo;			////texture color
uniform sampler2D tex_normal;			////texture normal

/*input variables*/
in vec3 vtx_pos;
in vec4 vtx_color;
in vec2 vtx_uv;

/*output variables*/
out vec4 frag_color;

void main()							
{		
	vec2 uv=vtx_uv;
	vec4 tex_color=texture(tex_albedo,uv);
	vec4 mat_color=vtx_color;

	frag_color=vec4(mat_color.rgb*tex_color.rgb, 1); // Phong
}	