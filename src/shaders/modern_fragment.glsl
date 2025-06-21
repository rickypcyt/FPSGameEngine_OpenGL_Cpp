#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform bool useLighting;

void main()
{
    if (useLighting) {
        // Ambient lighting
        float ambientStrength = 0.3;
        vec3 ambient = ambientStrength * lightColor;
        
        // Diffuse lighting
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;
        
        // Specular lighting
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColor;
        
        // Combine lighting
        vec3 result = (ambient + diffuse + specular);
        FragColor = vec4(result, 1.0);
    } else {
        // Psychedelic color mode
        vec3 psychedelicColor = vec3(
            0.5 + 0.5 * sin(gl_FragCoord.x * 0.01 + gl_FragCoord.y * 0.01),
            0.5 + 0.5 * sin(gl_FragCoord.x * 0.015 + gl_FragCoord.y * 0.02),
            0.5 + 0.5 * sin(gl_FragCoord.x * 0.02 + gl_FragCoord.y * 0.015)
        );
        FragColor = vec4(psychedelicColor, 1.0);
    }
} 