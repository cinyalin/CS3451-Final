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
in vec4 vtx_color;

/*output variables*/
out vec4 frag_color;	/*or use gl_FragColor*/
in vec3 nml;
in vec3 vtx_pos;

void main()							
{	
	float theta=iTime*2.;

	vec3 light_pos=vec3(2*cos(theta),2*sin(theta),0);
	vec3 l=normalize(light_pos-vtx_pos);
	float l_dot_n=dot(l,nml);
	float diff=max(0.,l_dot_n);

	frag_color = vec4(diff*vtx_color.rgb,1.f);
}	