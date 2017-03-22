#version 400 core

in vec2 fragTexCoords;
uniform sampler2D colorTexture;
uniform sampler2D depthTexture;
uniform float iGlobalTime;

layout (location = 0) out vec4 colorOut;

const float near = 0.1;
const float far = 100;
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}
void main()
{
// Fog parameters, could make them uniforms and pass them into the fragment shader
//    float fog_maxdist = 8.0;
//    float fog_mindist = 0.1;
//
//    // Calculate fog
//    //vec4 depth = texture(depthTexture, fragTexCoords);
//    float dist = length(gl_FragCoord.xyz);
//    float fog_factor = (fog_maxdist - dist) /
//                      (fog_maxdist - fog_mindist);
//    fog_factor = clamp(fog_factor, 0.0, 1.0);

    // TODO grab color based on depth
    vec2 texturePos = vec2(iGlobalTime, 0.5);
    vec4 color = texture(colorTexture, texturePos);

    float density = 0.1;
    const float LOG2 = 1.442695;
    float z = -(texture(depthTexture, fragTexCoords).b);
    float fog_factor = exp2(-density *
                       density *
                       z *
                       z *
                       LOG2 );
    fog_factor = clamp(fog_factor, 0.0, 1.0);
	fog_factor =  1 - fog_factor;
    vec4 clear_color = vec4(0.0, 0.0, 0.0, 0.0);
    colorOut = mix(clear_color, color, fog_factor);
}