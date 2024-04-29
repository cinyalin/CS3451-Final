#version 330 core

uniform samplerCube skybox;
in vec3 vtx_model_position;
out vec4 frag_color;

void main()
{
    // skybox is surounding the NDC cube, so we can use the vertex position as the direction
    vec3 color = texture(skybox, vtx_model_position).rgb;
    frag_color = vec4(color, 1.0);
}
