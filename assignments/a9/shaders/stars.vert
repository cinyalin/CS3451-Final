#version 330 core

layout(location = 0) in vec4 pos;			/*vertex position*/
layout(location = 1) in vec4 uv; 			/*vertex uv*/

out vec2 fragCoord;

uniform vec2 iResolution;

out vec3 vtx_pos;
out vec2 vtx_uv;

void main()
{
    vtx_pos = pos.xyz;
    vtx_uv = uv.xy;
    gl_Position = vec4(pos.xyz, 1.0);

    fragCoord = (0.5 * gl_Position.xy + vec2(0.5)) * iResolution;
}