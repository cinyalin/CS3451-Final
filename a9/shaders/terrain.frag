#version 330 core

layout (std140) uniform camera
{
	mat4 projection;
	mat4 view;
	mat4 pvm;
	mat4 ortho;
	vec4 position;
};

/* set light ubo. do not modify.*/
struct light
{
	ivec4 att; 
	vec4 pos; // position
	vec4 dir;
	vec4 amb; // ambient intensity
	vec4 dif; // diffuse intensity
	vec4 spec; // specular intensity
	vec4 atten;
	vec4 r;
};
layout(std140) uniform lights
{
	vec4 amb;
	ivec4 lt_att; // lt_att[0] = number of lights
	light lt[4];
};

uniform float iTime;
uniform mat4 model;		/*model matrix*/

in vec3 vtx_pos;

out vec4 frag_color;


uniform vec3 ka;            /* object material ambient */
uniform vec3 kd;            /* object material diffuse */
uniform vec3 ks;            /* object material specular */
uniform float shininess;    /* object material shininess */

vec2 hash2(vec2 v)
{
	vec2 rand = vec2(0,0);
	rand.x = fract(sin(dot(v.xy ,vec2(.9898,.233))) * .5453);
	rand.y = 52.5 * fract(v.y * 0.31 + dot(v.xy, vec2(0.31, 0.113)));
	rand = -1.0 + 3.1 * fract(rand.x * rand.y * rand.yx);
	return rand;
}

float perlin_noise(vec2 v) 
{
    float noise = 0;
	vec2 i = floor(v);
    vec2 f = fract(v);
    vec2 m = f*f*(3.0-2.0*f);
	
	noise = mix( mix( dot( hash2(i + vec2(0.0, 0.0)), f - vec2(0.0,0.0)),
					 dot( hash2(i + vec2(1.0, 0.0)), f - vec2(1.0,0.0)), m.x),
				mix( dot( hash2(i + vec2(0.0, 1.0)), f - vec2(0.0,1.0)),
					 dot( hash2(i + vec2(1.0, 1.0)), f - vec2(1.0,1.0)), m.x), m.y);
	return noise;
}

float noiseOctave(vec2 v, int num)
{
	float sum = 0;
	for(int i =0; i<num; i++){
		sum += pow(2,-1*i) * perlin_noise(pow(2,i) * v);
	}
	return sum;
}

float height(vec2 v){
    float h = 0;
	h = (1 - abs(noiseOctave(v, 10))) * 0.05;
	//if(h<0) h *= .5;
	return h;
}

vec3 compute_normal(vec2 v, float d)
{	
	vec3 normal_vector = vec3(0,0,0);
	vec3 right = vec3(v.x + d, v.y, height(vec2(v.x + d, v.y)));
	vec3 left = vec3(v.x - d, v.y, height(vec2(v.x - d, v.y)));
	vec3 forward = vec3(v.x, v.y + d, height(vec2(v.x, v.y + d)));
	vec3 backward = vec3(v.x, v.y - d, height(vec2(v.x, v.y - d)));

	vec3 x = normalize(right - left);
	vec3 y = normalize(forward - backward);

    normal_vector = normalize(cross(x, y));
	return normal_vector;
}

vec4 shading_phong(light li, vec3 e, vec3 p, vec3 s, vec3 n) 
{
    vec3 v = normalize(e - p);
    vec3 l = normalize(s - p);
    vec3 r = normalize(reflect(-l, n));

    vec3 ambColor = ka * li.amb.rgb;
    vec3 difColor = kd * li.dif.rgb * max(0., dot(n, l));
    vec3 specColor = ks * li.spec.rgb * pow(max(dot(v, r), 0.), shininess);

    return vec4(ambColor + difColor + specColor, 1);
}

// Draw the terrain
vec3 shading_terrain(vec3 pos) {
	vec3 n = compute_normal(pos.xy, 0.01);
	vec3 e = position.xyz;
	vec3 p = pos.xyz;
	vec3 s = lt[0].pos.xyz;

    // n = normalize((model * vec4(n, 0)).xyz);
    // p = (model * vec4(p, 1)).xyz;

    vec3 color = shading_phong(lt[0], e, p, s, n).xyz;

	float h = pos.z; // + .8;
	//h = clamp(h, 0.0, 1.0);
	vec3 emissiveColor = mix(vec3(.1, .4, .5), vec3(.25, .45, .56), h - 0.6);
	if (kd[0] == 0.8f) {
		emissiveColor = vec3(1);
	}
	if (kd[0] == 0.6f) {
		emissiveColor = mix(vec3(.4,.6,.2), vec3(.4,.3,.2), h);
	}

	return color * emissiveColor;
}

void main()
{
    frag_color = vec4(shading_terrain(vtx_pos), 1.0);
}
