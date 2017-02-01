#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

//uniform vec3 sun_pos;//sun position in world space

out vec3 pos;
//out vec3 sun_norm;

void main(){
    gl_Position = P * V * M *  position;
    pos = vec3(V * M * position);

    //sun_norm = normalize(sun_pos);
}
