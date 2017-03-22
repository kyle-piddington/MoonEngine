#version 400

in vec2 uv;
uniform sampler2D hdrTexture;

out vec4 color;

uniform float exposure;

// from https://www.shadertoy.com/view/lslGzl
vec3 whitePreservingLumaBasedReinhardToneMapping(vec3 color)
{
	float white = 2.0;
	float luma = dot(color, vec3(0.2126, 0.7152, 0.0722));
	float toneMappedLuma = luma * (1.0 + luma / (white*white)) / (1.0 + luma);
	color *= toneMappedLuma / luma;
	//color = pow(color, vec3(1. / gamma));
	return color;
}

void main()
{

    vec3 hdrColor = texture(hdrTexture, uv).rgb;

    // Exposure tone mapping
    vec3 mapped = whitePreservingLumaBasedReinhardToneMapping(hdrColor);
    //mapped = vec3(1.0) - exp(-hdrColor * exposure);
    // Gamma correction
    //mapped = pow(mapped, vec3(1.0 / gamma));

    color = vec4(mapped, 1.0);
}