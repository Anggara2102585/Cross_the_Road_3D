#version 450 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 aNormal;

out vec2 fragmentTexCoords;
out vec3 fragPos;
out vec3 normal;
out float visibility;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const float density = 0.015;
const float gradient = 4.5;

void main()
{
    vec4 worldPosition = model * vec4(vertexPosition, 1.0);
    vec4 positionRelativeToCam = view * worldPosition;
    gl_Position = projection * positionRelativeToCam;

    // Lighting
    fragmentTexCoords = texCoord;
    fragPos = vec3(worldPosition);
    normal = mat3(transpose(inverse(model))) * aNormal;

    // Fog
    float distance = length(positionRelativeToCam.xyz);
    visibility = exp(-pow((distance*density),gradient));
    visibility = clamp(visibility, 0.0, 1.0);   // not really necessary
}