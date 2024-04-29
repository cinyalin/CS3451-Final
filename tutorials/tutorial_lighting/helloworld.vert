/*Hello vertex shader!*/

#version 330 core

/*default camera matrices --- do not modify*/
layout (std140) uniform camera
{
	mat4 projection;	/*camera's projection matrix*/
	mat4 view;			/*camera's view matrix*/
	mat4 pvm;			/*camera's projection*view*model matrix*/
	mat4 ortho;			/*camera's ortho projection matrix*/
	vec4 position;		/*camera's position in world space*/
};

/*input variables from CPU*/
layout (location=0) in vec4 pos;			/*vertex position*/
layout (location=1) in vec4 v_color;		/*vertex color*/
layout (location=2) in vec4 normal;			/*vertex normal*/

/*output variables*/
out vec4 vtx_color;
out vec3 nml;
out vec3 vtx_pos;

////TODO: define your own data channel(s) to send the normal attribute from the vertex shader to the fragment shader

uniform float iTime;

void main()												
{
	float theta=iTime*2.;

	mat4 rotation=
	mat4(cos(theta),sin(theta),0.,0.,
		 -sin(theta),cos(theta),0.,0.,
		 0.,0.,1.,0.,
		 0.,0.,0.,1.);

	/*camera-transformed position. do not modify.*/
	vec4 rot_pos=rotation*vec4(pos.xyz,1.f);

	gl_Position=pvm*rot_pos;

	nml=(rotation*vec4(normal.xyz,1.)).xyz;

	vtx_color=vec4(v_color.rgb,1.f);
	vtx_pos=(rotation*vec4(pos.xyz,1.)).xyz;
}	