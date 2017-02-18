#version 400 core

in vec3 fragPos;
in vec3 sunPos;

uniform sampler2D skycolor;
uniform sampler2D sun;

uniform float iGlobalTime;

out vec3 color;

void main()
{
    vec3 normalPos = normalize(fragPos);

    float horizon = max(normalPos.y, 0.01);
    vec2 texturePos = vec2(iGlobalTime, horizon);

    color = texture(skycolor, texturePos).rgb;

    //Sun
    float radius = length(normalPos - sunPos);
    if (radius < 0.05){
        float time = clamp(sunPos.y, 0.01, 1);
        radius = radius / 0.05;
        if (radius < 1.0 - 0.05) {//small bias to avoid flickering
            //We read the alpha value from a texture where x = radius and y = height in the sky
            vec4 sun_color = texture(sun, vec2(radius, time));
            color = mix(color, sun_color.rgb, sun_color.a);
        }
    }
}
