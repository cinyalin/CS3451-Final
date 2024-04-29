/*Hello fragment shader!*/

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

/* Passed time from the begining of the program */ 
uniform float iTime;

/*input variables*/
////TODO: define your own data channel(s) to receive the normal attribute from the vertex shader

/*output variables*/
out vec4 frag_color;	/*or use gl_FragColor*/
in vec3 nml;

void main()							
{	
	frag_color = vec4(nml.rgb,1.f);
}	