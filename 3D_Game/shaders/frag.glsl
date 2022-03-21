#version 460 core
out vec4 FragColor;
flat in vec4 fColor;
in vec3 Normal;
in vec3 FragPos;

//uniform vec3 lightPos;

void main()
{
    vec3 lightPos = vec3(-3.0, 2.0, 3.0);
    vec3 lightColor = vec3(235.0/255, 192.0/255, 52.0/255);
    float ambientStrength = 0.1;
    
    vec3 ambient = ambientStrength * lightColor;
    
    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = fColor.xyz * (ambient + diffuse);


    FragColor = vec4(result, 1.0);
}