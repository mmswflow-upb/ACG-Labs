#version 400

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 texCoord;

out vec2 textureCoord;
out vec3 norm;
out vec3 fragPos;

uniform mat4 MVP;
uniform mat4 model;
uniform float time;

void main()
{
    // Apply mountain-like effect using sine wave to adjust height
    float Ai = 2.0;
    float Wi = 2.0 * 3.14159 / 10.0; // Frequency of the wave
    float Di = 1.0; // Wave direction multiplier
    float phi_i = 0.0; // Phase, kept zero for simplicity
    
    vec3 modifiedPos = pos;
    modifiedPos.y += 2.0 * Ai * (sin(time + Di * (pos.x + pos.z) * Wi + phi_i) + 1.0) / 2.0;

    textureCoord = texCoord;
    fragPos = vec3(model * vec4(modifiedPos, 1.0f));
    norm = mat3(transpose(inverse(model))) * normals;
    gl_Position = MVP * vec4(modifiedPos, 1.0f);
}