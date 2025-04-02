#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec4 lightColor;
uniform vec4 objectColor;

void main()
{
    
    // ambient
    float ambientStrength = 1.0;
    vec4 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.5); 
    vec4 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 1.0;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec4 specular = specularStrength * spec * lightColor;  
        
    vec4 result = (ambient + diffuse + specular) * objectColor;
    FragColor = result;
} 