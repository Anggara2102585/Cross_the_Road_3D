#version 450 core

in vec2 fragmentTexCoords;
in vec3 fragPos;
in vec3 normal;
in float visibility;

uniform sampler2D basicTexture;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 skyColor;

out vec4 finalColor;

void main()
{
    // Ambient light
    float ambientStrength = 0.5;
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse light
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular light
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = specularStrength * spec * lightColor;

    // texture
    finalColor = texture(basicTexture, fragmentTexCoords);
    // lighing
    finalColor = vec4(ambient + diffuse + specular, 1.0) * finalColor;
    // fog and done
    finalColor = mix(vec4(skyColor, 1.0), finalColor, visibility); // vis 0 (finalColor), vis 1 (skyColor)
}