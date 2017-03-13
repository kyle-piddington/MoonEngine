#version 410 core
out float fragColor;

uniform sampler2D ssaoColor;
uniform vec2 screenSize;
uniform vec2 texelSize;

vec2 locTexCoord()
{
    return gl_FragCoord.xy/screenSize;
}

void main() {
	vec2 TexCoords = locTexCoord();
    float result = 0.0;
    for (int x = -2; x < 2; ++x) 
    {
        for (int y = -2; y < 2; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(ssaoColor, TexCoords + offset).r;
        }
    }
    fragColor = result / (4.0 * 4.0);
}  