#version 400 core

in vec2 UV;
in vec3 sunPos;

out vec4 color;

uniform sampler2D sun;
uniform float iGlobalTime;

void main()
{
    float border_size = 0.01;
    float disc_radius = 0.5;
    vec2 disc_center = vec2(0.5, 0.5);

    vec2 center = UV - disc_center;
    float dist = dot(center, center);
    if (dist > 0.50001) {
        discard;
    }

    vec4 suncolor = texture(sun, UV);//vec2(dist, 0.5));
    color = suncolor;
//
//
//    // Offset uv with the center of the circle.
//    vec2 offset = UV - disc_center;
//
//    float dist = sqrt(dot(offset, offset));
//    float edge = smoothstep(disc_radius + border_size, disc_radius - border_size, dist);
//    color = mix(vec4(0.0, 0.0, 0.0, 0.0), suncolor, edge);
//
//    if (color.a < 0.03) {
//        discard;
//    }

//
//
//    float distance = distance(gl_FragCoord.xy, UV / 2.0);
//    color.xyz += (1.0 - distance / 256.0) * suncolor.xyz;
}