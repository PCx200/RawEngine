#version 400 core
out vec4 FragColor;
in vec3 fNor;
in vec2 uv;
uniform sampler2D text;
uniform float thickness = 1.0;
uniform float clarity = 1.0;

void main()
{
    float offset = 0.001 * thickness;
    vec2 offsets[9] = vec2[] ( vec2(-offset, offset), vec2(0.0, offset), vec2(offset, offset),
                               vec2(-offset, 0.0), vec2(0.0, 0.0), vec2(offset, 0.0),
                               vec2(-offset, -offset), vec2(0.0, -offset), vec2(offset, -offset)
    );
    float kernel[9] = float[] ( 1.0, 1.0, 1.0,
                                1.0, -8.0, 1.0,
                                1.0, 1.0, 1.0
    );
    vec4 result = vec4(0.0);

    for(int i = 0; i < 9; i++)
    {
        result += texture(text, uv + offsets[i]) * kernel[i] * clarity;
    }
    result.a = 1.0;
    FragColor = result;
}