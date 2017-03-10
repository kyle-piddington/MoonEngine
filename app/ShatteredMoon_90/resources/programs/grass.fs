#version 400

in vec2 UV;
out vec4 color;
uniform sampler2D diffuse;

void main()
{
    float alphaThresh = texture(diffuse, UV).a;
    if (alphaThresh < 0.2) {
        discard;
    }
    color = mix(vec4(0.5,0.6,0.0,1.0), vec4(0.3,0.2,0.2,1.0), 1-UV.x);
}