#include "GLNormalMapCreator.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <random>
#include <algorithm>
#include "Util/MathUtil.h"
#include "Util/Logger.h"
using namespace MoonEngine;
//http://www.flipcode.com/archives/Calculating_Vertex_Normals_for_Height_Maps.shtml
GLNormalMapCreator::GLNormalMapCreator()
{

}

float sampleAO(IHeightmapSource * heightmap,  glm::vec2 sampleCenter,const MapDimensions & mDims)
{
	int rasterSizeX = heightmap->getSizeX();
	int rasterSizeY = heightmap->getSizeZ();
	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // random floats between 0.0 - 1.0
	std::default_random_engine generator;
	float center = heightmap->getHeightAtFloat(sampleCenter.x, sampleCenter.y);
	float occlusion = 0.0f;
	float radius = 0.5f;
	int numSamples = 64;
	for (GLuint i = 0; i < numSamples; ++i)
	{
	    glm::vec3 sample(
	        randomFloats(generator) * 2.0 - 1.0, 
	        randomFloats(generator) * 2.0 - 1.0, 
	        randomFloats(generator) * 2.0 - 1.0
	    );
	    sample  = glm::normalize(sample);
	    sample *= randomFloats(generator);
	    float scale = (float)i / numSamples; 
	    scale   = MathUtil::lerp(0.1f, 1.0f, scale * scale);

	    sample.x *=  scale * radius *  (float)rasterSizeX/mDims.size.x;
	    sample.y *=  scale * radius * 65535.f/(float)mDims.size.y;
	    sample.z *=  scale * radius * (float)rasterSizeY/mDims.size.x;
	    sample+= glm::vec3(sampleCenter.x, center, sampleCenter.y);

	    float sampleDepth = heightmap->getHeightAtFloat(sample.x, sample.z);
	    float rangeCheck = MathUtil::smoothstep(0.0, 1.0, radius / fabsf(center - sampleDepth));
		occlusion       += (sampleDepth >= sample.y);       
   		
	}
	
	return std::max(0.0f,1 - occlusion/numSamples);
}

std::shared_ptr<GLTexture> GLNormalMapCreator::GenerateNormalMap(IHeightmapSource * heightmap, const GLTextureConfiguration  & newTextureConf, const MapDimensions & mDims)
{
	int rasterSizeX = heightmap->getSizeX();
	int rasterSizeY = heightmap->getSizeZ();
	float xzScale =  mDims.size.x/rasterSizeX;
	float yScale = (float)mDims.size.y / 65535.f;
	unsigned char * dataBuffer = new unsigned char[ 4 * newTextureConf.getWidth() * newTextureConf.getHeight()];
	for(int i = 0; i < newTextureConf.getWidth(); i++)
	{
		for(int j = 0; j < newTextureConf.getHeight(); j++)
		{
			glm::vec2 center = glm::vec2((float)i/newTextureConf.getWidth() * rasterSizeX, (float)j/newTextureConf.getHeight() * rasterSizeY);
			float xOff = 1.0;
			//Calculate gradient:
			float right = heightmap->getHeightAtFloat(center.x + xOff, center.y);
			float left = heightmap->getHeightAtFloat(center.x - xOff, center.y);
			float up = heightmap->getHeightAtFloat(center.x, center.y + xOff);
			float down = heightmap->getHeightAtFloat(center.x, center.y - xOff);
			float dfdx =  right - left;
			float dfdy = up - down;
			glm::vec3 normal = glm::vec3(-yScale*dfdx,2*xzScale,yScale*dfdy);
			float ao = sampleAO(heightmap, center, mDims);
			

			//Calculate AO using the vector and a random sampling to decide above/below.
			
			normal = glm::normalize(normal);
			normal = normal * 0.5f + 0.5f;
			dataBuffer[4*(j*newTextureConf.getWidth() + (i)) + 0] = (int)(255 * normal.x);
			dataBuffer[4*(j*newTextureConf.getWidth() + (i)) + 1] = (int)(255 * normal.y);
			dataBuffer[4*(j*newTextureConf.getWidth() + (i)) + 2] = (int)(255 * normal.z);
			dataBuffer[4*(j*newTextureConf.getWidth() + (i)) + 3] = (int)(255 * ao);
			
				
		}
	}

	/*@TODO: FIX THIS WHEN MERGING WITH RENDER*/
	std::shared_ptr<GLTexture> normalTexture = std::make_shared<GLTexture>(1);
	normalTexture->init(dataBuffer, newTextureConf);
	return normalTexture;

}