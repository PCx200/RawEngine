#version 400 core
out vec4 FragColor;
in vec3 fPos;
in vec3 fNor;
in vec2 uv;

uniform sampler2D text;
uniform float pixelSize;
uniform vec2 screenSize;

void main()
{

    vec2 uvCentered = uv + vec2(0.5);
    float px = screenSize.x / pixelSize;
    float py = screenSize.y / pixelSize;
    vec2 pixelCount = vec2(px, py);

    vec2 uvPixel = uvCentered * pixelCount;

    uvPixel = floor(uvPixel);

    vec2 uvSnapped = uvPixel / pixelCount;

    vec2 uvFinal = uvSnapped - vec2(0.5);

   FragColor = texture(text,uvFinal);
}