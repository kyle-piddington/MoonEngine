#version 400 core
in vec3 fragPos;
in vec3 fragNor;
in vec2 fragTex;

layout (location = 0) out vec4 posOut;
layout (location = 1) out vec4 colorOut;
layout (location = 2) out vec4 normalOut;

uniform mat4 V;

uniform sampler2D diffuse;

vec3 blue = vec3(50.0/255.0, 150.0/255.0, 200.0/255.0);
vec3 lightBlue = vec3(200.0/255.0, 235.0/255.0, 255.0/255.0);

vec3 black = vec3(0.0);
vec3 white = vec3(1.0);

vec3 gray(in vec3 color)
{
  float average = (color.r + color.g + color.b) / 3.0;
  return vec3(average);
}

void main()
{
    posOut = vec4(fragPos, 1.0);
    vec3 color = texture(diffuse, fragTex).rgb;
    colorOut.rgb = gray(color) + 0.5;

	vec3 viewDir = normalize(vec3(0, 0, 1) - fragPos);

    float edge = dot(viewDir, normalize(fragNor));
    colorOut.rgb *= blue * edge + lightBlue * 2 * (1 - edge);

    colorOut.a = 0.1; // specular
    normalOut.xyz = normalize(fragNor);
    normalOut.a = 1.0; // NO SHADOW WEIRDNESS
}