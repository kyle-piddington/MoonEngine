#version 330 core
in vec3 fragPos;
in vec3 fragTex;
out vec4 color;
uniform vec3 tint;
uniform sampler2D heightmap;

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{

	color = vec4((vec3(fragTex.xy,0) * 0.5 + 0.5) * texture(heightmap,fragTex.xy).r ,1.0);
}
