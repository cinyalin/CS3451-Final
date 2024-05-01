#version 300 es

in vec3 aPosition;
in vec2 aTexCoord;

out vec2 texCoord;

uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;

void main() {
    gl_Position = uProjectionMatrix * uModelViewMatrix * vec4(aPosition, 1.0);
    texCoord = aTexCoord;
}