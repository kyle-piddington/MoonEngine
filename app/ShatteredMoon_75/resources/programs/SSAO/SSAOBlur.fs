#version 410 core
out float finalColor;

uniform sampler2D ssaoColor;
uniform vec2 screenSize;

vec2 locTexCoord()
{
    return gl_FragCoord.xy/screenSize;
}

void main() {
	vec2 texelSize = 1 / screenSize;
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
   finalColor = result / (4.0 * 4.0);
  // finalColor = texture(ssaoColor, TexCoords).r;
}  