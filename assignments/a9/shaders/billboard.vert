#version 330 core

/*default camera matrices. do not modify.*/
layout(std140) uniform camera {
    mat4 projection;	/*camera's projection matrix*/
    mat4 view;			/*camera's view matrix*/
    mat4 pvm;			/*camera's projection*view*model matrix*/
    mat4 ortho;			/*camera's ortho projection matrix*/
    vec4 position;		/*camera's position in world space*/
};

uniform mat4 model;		/*model matrix*/

/*input variables*/
layout(location = 0) in vec4 pos;			/*vertex position*/
layout(location = 1) in vec4 v_color;		/*vertex color*/
layout(location = 2) in vec4 normal;		/*vertex normal*/
layout(location = 3) in vec4 uv; 			/*vertex uv*/
layout(location = 4) in vec4 tangent;	    /*vertex tangent*/

/*output variables*/
out vec4 vtx_color;
out vec3 vtx_normal; // world space normal
out vec3 vtx_position; // world space position
out vec3 vtx_model_position; // model space position
out vec2 vtx_uv;
out vec3 vtx_tangent;

void main() {
    vec4 worldPos = model * vec4(pos.xyz, 1.);
    // ! do not support non-uniform scale
    vec4 worldNormal = model * vec4(normal.xyz, 0.);
    vec4 worldTangent = model * vec4(tangent.xyz, 0.);

    // billboard effect begin, https://community.khronos.org/t/opengl-billboard/108352
    vec3 noTransPos = mat3(model) * pos.xyz; // world position without translation
    mat4 inverseViewMat = inverse(view);
    inverseViewMat[3].xyz = model[3].xyz;
    mat4 viewMat = view * inverseViewMat;
    // billboard effect end

    vtx_normal = normalize(worldNormal.xyz);
    vtx_model_position = pos.xyz;
    vtx_position = worldPos.xyz;
    vtx_color = vec4(v_color.rgb, 1.);
    vtx_uv = uv.xy;
    vtx_tangent = worldTangent.xyz;

    gl_Position = projection * viewMat * vec4(noTransPos.xyz, 1.); // billboard effect
}