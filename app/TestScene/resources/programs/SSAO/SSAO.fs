#version 410 core
out float ssaoColor;

uniform sampler2D positionTex;
uniform sampler2D normalTex;
uniform sampler2D noiseTex;

uniform vec2 screenSize;
uniform vec3 kernel[64];
uniform mat4 P;

// parameters (you'd probably want to use them as uniforms to more easily tweak the effect)
float radius = 0.5;
float bias = 0.025;
int kernelSize = 64;
vec2 noiseScale = vec2(screenSize.x/4.0, screenSize.y/4.0); 

vec2 locTexCoord()
{
    return gl_FragCoord.xy/screenSize;
}

void main()
{
    // Get input for SSAO algorithm
    vec2 TexCoords = locTexCoord();
    vec3 fragPos = texture(positionTex, TexCoords).xyz;
    vec3 normal = normalize(texture(normalTex, TexCoords).rgb);
    vec3 randomVec = normalize(texture(noiseTex, TexCoords * noiseScale).xyz);
    // Create TBN change-of-basis matrix: from tangent-space to view-space
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    // Iterate over the sample kernel and calculate occlusion factor
    float occlusion = 0.0;
    vec3 sampl = vec3(0,0,0);
    for(int i = 0; i < kernelSize; ++i)
    {
        sampl = kernel[i]; // From tangent to view-space
        sampl = fragPos + sampl * radius; 
        
        vec4 offset = vec4(sampl, 1.0);
        offset = P * offset; // from view to clip-space
        offset.xyz /= offset.w; // perspective divide
        offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0
        
        float sampleDepth = texture(positionTex, offset.xy).z; // Get depth value of kernel sample
        
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= sampl.z + bias ? 1.0 : 0.0) * rangeCheck;           
    }
    occlusion = 1.0 - (occlusion / kernelSize);
    
    ssaoColor = occlusion;
}