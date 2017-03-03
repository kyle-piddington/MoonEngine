#version 400

in vec2 uv;
uniform sampler2D texture;

out vec4 color;

//
void main()
{
    const float gamma = 2.2;
    vec3 hdrColor = texture(texture, uv).rgb;

    vec3 mapped = hdrColor / (hdrColor + vec3(1.0));

    mapped = pow(mapped, vec3(1.0 / gamma));

    color = vec4(mapped, 1.0);
}