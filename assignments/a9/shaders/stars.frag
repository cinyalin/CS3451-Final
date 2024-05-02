#version 330 core

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;

in vec2 fragCoord; 

#define Time (iTime*1.0) 
#define DURATION 3.
#define Gravity 0.7  

const vec2 g = vec2(.0, -Gravity); /* gravity */

uniform sampler2D tex_buzz; 

in vec3 vtx_pos; // [-1, 1]
in vec2 vtx_uv; // [0, 1]

out vec4 frag_color;

#define NUM_STAR 100.

// return random vec2 between 0 and 1
vec2 hash2d(float t)
{
    t += 1.;
    float x = fract(sin(t * 674.3) * 453.2);
    float y = fract(sin((t + x) * 714.3) * 263.2);

    return vec2(x, y);
}

vec4 renderParticle(vec2 uv, vec2 pos, float brightness, vec3 color)
{
    float d = length(uv - pos);
    return vec4(brightness / d * color, brightness / d);
}

vec4 renderStars(vec2 uv)
{
    vec4 fragColor = vec4(0.0);

    float t = iTime;
    for(float i = 0.; i < NUM_STAR; i++)
    {
        vec2 pos = hash2d(i) * 2. - 1.; // map to [-1, 1]
        float brightness = .005;
        brightness *= sin(1.5 * t + i) * .5 + .5; // flicker
        vec3 color = vec3(0.92, 0.71, 0.30);

        fragColor += renderParticle(uv, pos, brightness, color);
    }

    return fragColor;
}

void main()
{
    vec4 outputColor = renderStars(vtx_pos.xy);
    vec2 fragPos = (fragCoord - .5 * iResolution.xy) / iResolution.y;

    // vec2 uv = vec2(vtx_uv.x, -vtx_uv.y);
    // vec3 buzzColor = texture(tex_buzz, uv).xyz;

    vec2 initPos = vec2(-0.5, -0.5);
    vec2 initVel = vec2(0.4, 1.);
    float t = mod(Time, DURATION);
    float brightness = 0.05;
    vec3 color = vec3(0.92, 0.71, 0.30);
    vec4 fragColor = renderStars(fragPos);

    frag_color = outputColor;
}