#version 400

in vec2 uv;
uniform sampler2D hdrTexture;

out vec4 color;

uniform float exposure;

void main()
{
    const float gamma = 2.2;

    vec3 hdrColor = texture(hdrTexture, uv).rgb;

    // Exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    // Gamma correction
    mapped = pow(mapped, vec3(1.0 / gamma));

    color = vec4(mapped, 1.0);
}