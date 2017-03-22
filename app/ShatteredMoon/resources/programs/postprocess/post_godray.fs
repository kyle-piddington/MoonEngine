#version 330 core
in vec2 fragTexCoords;
out vec4 color;

const int NUM_SAMPLES = 32;
const float Density = 0.6;
const float Decay = 0.8;
const float Weight = 1;
const float Exposure = 1;

uniform vec4 ScreenLightPos;
uniform sampler2D occlusionLightTexture;
void main()
{

   vec2 texCoords = fragTexCoords;
   vec2 deltaTexCoord = (fragTexCoords - (ScreenLightPos.xy * 0.5 + 0.5));
   
   deltaTexCoord *= 1.0/NUM_SAMPLES * Density;
   vec3 accumColor = texture(screenTexture, fragTexCoords).rgb;
   float illuminationDecay = 1.0;
   vec3 accumColor = vec4(0.0);
   for(int iter = 0; iter < NUM_SAMPLES; iter++)
   {
   		texCoords -= deltaTexCoord;
   		vec3 sample =  texture(occlusionLightTexture, texCoords).rgb;;
   		sample *= illuminationDecay * Weight;
   		accumColor += sample;
   		illuminationDecay *= Decay;
   }
   color = vec4(accumColor * Exposure, 1);


}