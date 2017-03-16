#version 410    

layout (location = 0) in vec3 Position; 
layout (location = 2) in vec2 vertTexCoord;

out vec2 TexCoords;
void main()
{          
    gl_Position = vec4(Position, 1.0);
    TexCoords = vertTexCoord;
}
