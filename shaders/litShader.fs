#version 400 core
struct Light{
    vec3 position;
    vec4 color;
    float radius;
};

uniform int lightCount;
uniform Light lights[10];

uniform float ambientIntensity;
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 speculaColor;
uniform float specularIntensity;

uniform vec3 cameraPos;

out vec4 FragColor;
in vec3 fPos;
in vec3 fNor;
in vec2 uv;

void main()
{
//FragColor=vec4(1,0,1,1);
    // just debug:
    //FragColor = vec4(lightDir,1); // TODO: light calculations (at least diffuse )
    vec3 normal = normalize(fNor);
    vec3 viewDir = normalize(cameraPos - fPos);

    vec3 result = ambientIntensity * ambientColor;

    for(int i = 0; i < lightCount ; i++)
    {
        vec3 lightDir = normalize(lights[i].position - fPos);
        vec3 halfwayDir = normalize(lightDir + viewDir);


        float dist = length(lights[i].position - fPos);

        float radius = lights[i].radius;

        //float attenuation = 1.0 / (0.001 + 0.001 * dist + 1.0 * dist * dist);
        float attenuation = clamp(1.0 - dist / radius, 0.0, 1.0);
        //vec3 ambient = ambientIntensity * ambientColor;

        vec3 diffuse = max(dot(lightDir, normal), 0) * vec3(lights[i].color) * diffuseColor;

        vec3 specular = pow(max(dot(normal, halfwayDir), 0), specularIntensity) * speculaColor;

        result += attenuation * (diffuse + specular);
    }

        FragColor = vec4(result, 1);
}