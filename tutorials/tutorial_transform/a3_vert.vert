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

uniform mat4 model;				/*model transform matrix*/

/*input variables from CPU*/
layout (location=0) in vec4 pos;			/*vertex position*/
layout (location=1) in vec4 v_color;		/*vertex color*/
layout (location=2) in vec4 normal;			/*vertex normal*/

/*output variables*/
out vec4 vtx_color;
out vec4 worldPos;
out vec3 fragNormal;

void main()												
{
	vtx_color = vec4(v_color.rgb,1.f); // vertex color
	vec4 worldNormal = model * vec4(normal.xyz, 0.f); 
	fragNormal = normalize(worldNormal.xyz); // world normal
	worldPos = model * vec4(pos.xyz, 1.f); // world position
	gl_Position = pvm * worldPos;
}	