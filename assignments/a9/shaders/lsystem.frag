#version 300 es

precision highp float;

in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D uTexture;
uniform float uTime;

#define ITERATIONS 5
#define AXIOM "F"
#define RULES "F->[F]+F--F+F+[+F]-F--"

void main() {
    vec2 uv = texCoord * 10.0; // scale up the texture coordinates
    vec3 color = vec3(0.0);

    // L-system implementation
    string axiom = AXIOM;
    string rules = RULES;
    string current = axiom;

    for (int i = 0; i < ITERATIONS; i++) {
        string next = "";
        for (int j = 0; j < current.length(); j++) {
            char c = current[j];
            if (c == 'F') {
                next += rules;
            } else {
                next += string(1, c);
            }
        }
        current = next;
    }

    // render the grass blades
    for (int i = 0; i < current.length(); i++) {
        char c = current[i];
        if (c == 'F') {
            // draw a grass blade
            vec2 offset = vec2(sin(uTime + i * 0.1), cos(uTime + i * 0.1)) * 0.1;
            color += texture(uTexture, uv + offset).rgb;
        } else if (c == '+') {
            // rotate the blade
            uv += vec2(0.1, 0.0);
        } else if (c == '-') {
            // rotate the blade
            uv -= vec2(0.1, 0.0);
        }
    }

    fragColor = vec4(color, 1.0);
}