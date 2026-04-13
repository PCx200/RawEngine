#version 400 core
out vec4 FragColor;

uniform bool intersects;

void main()
{
    if(intersects)
    {
        FragColor = vec4(1, 0, 0, 1);
    }
    else
    {
        FragColor = vec4(0, 1, 0, 1);
    }

}