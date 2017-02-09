#include "MeshCreator.h"

using namespace MoonEngine;
std::vector<std::shared_ptr<GLBuffer>> MeshCreator::backingBuffers;
std::vector<std::shared_ptr<GLVertexArrayObject>> MeshCreator::backingArrayObjects;


MeshInfo *  MeshCreator::CreateQuad(glm::vec2 lowerLeft, glm::vec2 upperRight, int divX, int divY)
{

    backingBuffers.push_back(std::make_shared<GLBuffer>(GL_ARRAY_BUFFER));
    backingBuffers.push_back(std::make_shared<GLBuffer>(GL_ELEMENT_ARRAY_BUFFER));

	GLBuffer * vertexBuffer = backingBuffers[backingBuffers.size() - 2].get();
	GLBuffer * indexBuffer = backingBuffers[backingBuffers.size() - 1].get();
	std::vector<float> dataBuffer;
	std::vector<unsigned short> indices;
	for(int row = 0; row < divY; row++)
	{
		for (int col = 0; col < divX; col++)
		{
			float u = row/(float)(divY - 1);
			float v = col/(float)(divX - 1);
			glm::vec3 pos;
			pos.x  = lowerLeft.x + (upperRight.x - lowerLeft.x) * u;
			pos.z = lowerLeft.y + (upperRight.x - lowerLeft.y) * v;
			
			dataBuffer.push_back(pos.x);
			dataBuffer.push_back(0);
			dataBuffer.push_back(pos.z);
            dataBuffer.push_back(u);
            dataBuffer.push_back(v);
            
		}
	}
	for(int row = 1; row < divY; row++)
	{
		for(int col = 1; col < divX; col++)
		{
    		indices.push_back(row * divX + col);
            
			indices.push_back((row-1) * divX + (col - 1));
            

            indices.push_back(row * divX + (col-1));
            
		
			indices.push_back((row - 1) * divX + col);
            
            indices.push_back((row-1) * divX + (col - 1));
            
			indices.push_back(row * divX + col);
            
            
				
		}
	}	

	vertexBuffer->setData(sizeof(float)*dataBuffer.size(), &(dataBuffer[0]), GL_STATIC_DRAW);
	indexBuffer->setData(sizeof(unsigned int)*indices.size(), &(indices[0]),GL_STATIC_DRAW);
	std::shared_ptr<GLVertexArrayObject> vertexArrayObject = std::make_shared<GLVertexArrayObject>();
	backingArrayObjects.push_back(vertexArrayObject);

	vertexArrayObject->bindVertexBuffer(
		GL_VERTEX_POSITION_ATTRIBUTE, 
		*vertexBuffer, 
		3,
		GL_FLOAT,
		false,
		sizeof(float)*5);

	vertexArrayObject->bindVertexBuffer(
		GL_VERTEX_TEXTURE_ATTRIBUTE, 
		*vertexBuffer, 
		2,
		GL_FLOAT,
		false,
		sizeof(float)*5,
		(GLvoid *)(sizeof(float)*3));

	vertexArrayObject->bindElementBuffer(*indexBuffer);
	//Create meshInfo
	MeshInfo * info = new MeshInfo();
	info->numVerts = dataBuffer.size() / 5;
	info->numTris = indices.size();
	info->indexDataOffset = (GLvoid *) 0;
	info->baseVertex = 0;
	info->setVertexArrayObject(vertexArrayObject.get());
	return info;
}

GridInfo MeshCreator::CreateGrid(glm::vec2 lowerLeft, glm::vec2 upperRight, int vertDim)
{

    if(!vertDim % 2)
    {
        assert(!"Grids must have an odd number of vertices");
    }
    backingBuffers.push_back(std::make_shared<GLBuffer>(GL_ARRAY_BUFFER));
    backingBuffers.push_back(std::make_shared<GLBuffer>(GL_ELEMENT_ARRAY_BUFFER));

    GLBuffer * vertexBuffer = backingBuffers[backingBuffers.size() - 2].get();
    GLBuffer * indexBuffer = backingBuffers[backingBuffers.size() - 1].get();
    std::vector<float> dataBuffer;
    std::vector<unsigned short> indices;
    for(int row = 0; row < vertDim; row++)
    {
        for (int col = 0; col < vertDim; col++)
        {
            float u = row/(float)(vertDim - 1);
            float v = col/(float)(vertDim - 1);
            glm::vec3 pos;
            pos.x  = lowerLeft.x + (upperRight.x - lowerLeft.x) * u;
            pos.z = lowerLeft.y + (upperRight.x - lowerLeft.y) * v;
            
            dataBuffer.push_back(pos.x);
            dataBuffer.push_back(0);
            dataBuffer.push_back(pos.z);
            dataBuffer.push_back(u);
            dataBuffer.push_back(v);
            
        }
    }
    GridInfo gridInfo;
    int halfd = 1 + (vertDim  - 1)/2 ; //Ceiling of halfd
    int fulld = vertDim;
    gridInfo.tlIdx = 0;
    //Bottom Left
    for(int row = 1; row < halfd ;  row++)
    {
        for(int col = 1; col < halfd; col++)
        {
            indices.push_back(row * vertDim + col);
            
            indices.push_back((row-1) * vertDim + (col - 1));
            

            indices.push_back(row * vertDim + (col-1));
            
        
            indices.push_back((row - 1) * vertDim + col);
            
            indices.push_back((row-1) * vertDim + (col - 1));
            
            indices.push_back(row * vertDim + col);
        }
    }
    gridInfo.blIdx = indices.size();
    for(int row = 1; row < halfd ;  row++)
    {
        for(int col = halfd; col < fulld; col++)
        {
            indices.push_back(row * vertDim + col);
            
            indices.push_back((row-1) * vertDim + (col - 1));
            

            indices.push_back(row * vertDim + (col-1));
            
        
            indices.push_back((row - 1) * vertDim + col);
            
            indices.push_back((row-1) * vertDim + (col - 1));
            
            indices.push_back(row * vertDim + col);
        }
    }
    gridInfo.trIdx = indices.size();
    for(int row = halfd; row < fulld ;  row++)
    {
        for(int col = 1; col < halfd; col++)
        {
            indices.push_back(row * vertDim + col);
            
            indices.push_back((row-1) * vertDim + (col - 1));
            

            indices.push_back(row * vertDim + (col-1));
            
        
            indices.push_back((row - 1) * vertDim + col);
            
            indices.push_back((row-1) * vertDim + (col - 1));
            
            indices.push_back(row * vertDim + col);
        }
    }
    gridInfo.brIdx = indices.size();
    for(int row = halfd; row < fulld ;  row++)
    {
        for(int col = halfd; col < fulld; col++)
        {
            indices.push_back(row * vertDim + col);
            
            indices.push_back((row-1) * vertDim + (col - 1));
            

            indices.push_back(row * vertDim + (col-1));
            
        
            indices.push_back((row - 1) * vertDim + col);
            
            indices.push_back((row-1) * vertDim + (col - 1));
            
            indices.push_back(row * vertDim + col);
        }
    }
    vertexBuffer->setData(sizeof(float)*dataBuffer.size(), &(dataBuffer[0]), GL_STATIC_DRAW);
    indexBuffer->setData(sizeof(unsigned int)*indices.size(), &(indices[0]),GL_STATIC_DRAW);
    std::shared_ptr<GLVertexArrayObject> vertexArrayObject = std::make_shared<GLVertexArrayObject>();
    backingArrayObjects.push_back(vertexArrayObject);
    vertexArrayObject->bindVertexBuffer(
        GL_VERTEX_POSITION_ATTRIBUTE, 
        *vertexBuffer, 
        3,
        GL_FLOAT,
        false,
        sizeof(float)*5);
    vertexArrayObject->bindVertexBuffer(
        GL_VERTEX_TEXTURE_ATTRIBUTE, 
        *vertexBuffer, 
        2,
        GL_FLOAT,
        false,
        sizeof(float)*5,
        (GLvoid *)(sizeof(float)*3));
    vertexArrayObject->bindElementBuffer(*indexBuffer);
    //Create meshInfo
    MeshInfo * info = new MeshInfo();
    info->numVerts = dataBuffer.size() / 5;
    info->numTris = indices.size();
    info->indexDataOffset = (GLvoid *) 0;
    info->baseVertex = 0;
    info->setVertexArrayObject(vertexArrayObject.get());
    gridInfo.meshInfo = info;
    gridInfo.gridSize = vertDim; 
    return gridInfo;

}