#version 400 core

uniform float ambientIntensity;
uniform vec4 lightColor;
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 speculaColor;
uniform float specularIntensity;

uniform vec3 cameraPos;
uniform vec3 lightPos;

out vec4 FragColor;
in vec3 fPos;
in vec3 fNor;
in vec2 uv;

void main()
{
//FragColor=vec4(1,0,1,1);
    // just debug:
    //FragColor = vec4(lightDir,1); // TODO: light calculations (at least diffuse )
    vec3 lightDir = normalize(lightPos - fPos);
    vec3 viewDir = normalize(cameraPos - fPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    vec3 normal = normalize(fNor);

    vec3 ambient = ambientIntensity * ambientColor;

    vec3 diffuse = max(dot(lightDir, normal), 0) * vec3(lightColor) * diffuseColor;

    vec3 specular = pow(max(dot(normal, halfwayDir), 0), specularIntensity) * speculaColor;

    vec3 ads = ambient + diffuse + specular;

    FragColor = vec4(ads, 1);

   //FragColor = vec4(fNor.x, fNor.y, fNor.z, 1);
}