#version 400 core

in vec2 UV;
in vec3 sunPos;

out vec4 color;

uniform sampler2D sun;
uniform float iGlobalTime;

void main()
{

    vec2 coord = 2.0 * UV - 1.0;
    float dist = dot(coord, coord);

    if (dist > 1.0001) {
        discard;
    }

	color = texture(sun, vec2(dist, iGlobalTime));
}