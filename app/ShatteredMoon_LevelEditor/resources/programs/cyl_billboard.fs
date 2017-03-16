#version 400

in vec2 UV;
out vec4 color;
uniform sampler2D diffuse;

void main()
{
    color = texture(diffuse, UV);
    if (color.a == 0.0) {
        discard;
    }
}