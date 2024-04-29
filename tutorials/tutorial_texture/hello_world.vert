/*This is your first vertex shader!*/

#version 330 core

#define PI 3.14159265

/*default camera matrices. do not modify.*/
layout (std140) uniform camera
{
	mat4 projection;	/*camera's projection matrix*/
	mat4 view;			/*camera's view matrix*/
	mat4 pvm;			/*camera's projection*view*model matrix*/
	mat4 ortho;			/*camera's ortho projection matrix*/
	vec4 position;		/*camera's position in world space*/
};

/*input variables*/


layout (location=0) in vec4 pos;		/*vertex position*/
layout (location=1) in vec4 color;		/*vertex color*/
layout (location=2) in vec4 normal;		/*vertex normal*/
layout (location=3) in vec4 uv;			/*vertex uv*/
layout (location=4) in vec4 tangent;		/*vertex tangent*/

/*output variables*/
out vec3 vtx_pos;
out vec4 vtx_color;
out vec2 vtx_uv;


/*
float height(vec2 xy)
{
	float h1=0.2*sin((xy.x*xy.x+xy.y*xy.y)*2.);
	float h2=0.1*sin((xy.x*xy.x+xy.y*xy.y)*4.);
	return h1+h2;
}
*/

vec2 hash2(vec2 v)
{
	vec2 rand = vec2(0,0);
	
	rand  = 50.0 * 1.05 * fract(v * 0.3183099 + vec2(0.71, 0.113));
    rand = -1.0 + 2 * 1.05 * fract(rand.x * rand.y * (rand.x + rand.y) * rand);

	return 0.1*rand;
}

float height(vec2 v)
{
	return hash2(v*10.).x+hash2(v*10.).y;
}

/*
float height(vec2 v)
{
	return .1*sin((v.x*v.x+v.y*v.y)*2.);
}
*/

void main()												
{
	/*camera-transformed position. do not modify.*/
	gl_Position=pvm*vec4(pos.xy,height(uv.xy),1.f);

	////TODO: add your operations on the out varialbes
	vtx_pos = pos.xyz;
	vtx_color=vec4(color.rgb,1.f);
	vtx_uv = uv.xy;
}	