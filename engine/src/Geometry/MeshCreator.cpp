#include "MeshCreator.h"

using namespace MoonEngine;
std::vector<std::shared_ptr<GLBuffer>> MeshCreator::backingBuffers;
std::vector<std::shared_ptr<GLVertexArrayObject>> MeshCreator::backingArrayObjects;


MeshInfo * MeshCreator::CreateQuad(glm::vec2 lowerLeft, glm::vec2 upperRight)
{

    backingBuffers.push_back(std::make_shared<GLBuffer>(GL_ARRAY_BUFFER));
    backingBuffers.push_back(std::make_shared<GLBuffer>(GL_ELEMENT_ARRAY_BUFFER));

    GLBuffer * vertexBuffer = backingBuffers[backingBuffers.size() - 2].get();
    GLBuffer * indexBuffer = backingBuffers[backingBuffers.size() - 1].get();

    std::vector<float> dataBuffer;
    int indices[] = {0, 3, 1, 0, 2, 3};
    /**
     * 1        3
     * |		|
     * |		|
     * |		|
     * 0--------2
     */
    dataBuffer.push_back(lowerLeft.x);
    dataBuffer.push_back(lowerLeft.y);
    dataBuffer.push_back(0);
    //Tex coords
    dataBuffer.push_back(0);
    dataBuffer.push_back(0);

    dataBuffer.push_back(lowerLeft.x);
    dataBuffer.push_back(upperRight.y);
    dataBuffer.push_back(0);
    dataBuffer.push_back(0);
    dataBuffer.push_back(1);


    dataBuffer.push_back(upperRight.x);
    dataBuffer.push_back(lowerLeft.y);
    dataBuffer.push_back(0);
    dataBuffer.push_back(1);
    dataBuffer.push_back(0);

    dataBuffer.push_back(upperRight.x);
    dataBuffer.push_back(upperRight.y);
    dataBuffer.push_back(0);
    dataBuffer.push_back(1);
    dataBuffer.push_back(1);


    vertexBuffer->setData(sizeof(float) * dataBuffer.size(), &(dataBuffer[0]), GL_STATIC_DRAW);
    indexBuffer->setData(sizeof(indices), indices, GL_STATIC_DRAW);
    std::shared_ptr<GLVertexArrayObject> vertexArrayObject = std::make_shared<GLVertexArrayObject>();
    backingArrayObjects.push_back(vertexArrayObject);
    vertexArrayObject->bindVertexBuffer(
        GL_VERTEX_POSITION_ATTRIBUTE,
        *vertexBuffer,
        3,
        GL_FLOAT,
        false,
        sizeof(float) * 5
    );
    vertexArrayObject->bindVertexBuffer(
        GL_VERTEX_TEXTURE_ATTRIBUTE,
        *vertexBuffer,
        2,
        GL_FLOAT,
        false,
        sizeof(float) * 5,
        (GLvoid *) (sizeof(float) * 3)
    );
    vertexArrayObject->bindElementBuffer(*indexBuffer);
    //Create meshInfo
    MeshInfo * info = new MeshInfo();
    info->numVerts = 4;
    info->numTris = 6;
    info->indexDataOffset = (GLvoid *) 0;
    info->baseVertex = 0;
    info->setVertexArrayObject(vertexArrayObject.get());
    return info;
}