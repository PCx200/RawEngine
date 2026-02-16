#version 400 core
struct Light{
    vec3 position;
    vec4 color;
    float radius;
};

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;

uniform int lightCount;
uniform Light lights[10];

uniform float ambientIntensity;
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 speculaColor;
uniform float specularIntensity;

uniform vec3 cameraPos;

uniform sampler2D text;

out vec4 FragColor;
in vec3 fPos;
in vec3 fNor;
in vec2 uv;

void main()
{
    vec3 normal = normalize(fNor);
    vec3 viewDir = normalize(cameraPos - fPos);

    vec3 result = ambientIntensity * ambientColor * material.ambient;

    for(int i = 0; i < lightCount ; i++)
    {
        vec3 lightDir = normalize(lights[i].position - fPos);
        vec3 halfwayDir = normalize(lightDir + viewDir);


        float dist = length(lights[i].position - fPos);

        float radius = lights[i].radius;

        //float attenuation = 1.0 / (0.001 + 0.001 * dist + 1.0 * dist * dist);
        float attenuation = clamp(1.0 - dist / radius, 0.0, 1.0);
        //vec3 ambient = ambientIntensity * ambientColor;

        vec3 diffuse = max(dot(lightDir, normal), 0) * vec3(lights[i].color) * diffuseColor * material.diffuse;

        vec3 specular = pow(max(dot(normal, halfwayDir), 0), specularIntensity) * speculaColor * material.specular;

        result += attenuation * (diffuse + specular);
    }

        FragColor = vec4(result + vec3(texture(text, uv)), 1);
}