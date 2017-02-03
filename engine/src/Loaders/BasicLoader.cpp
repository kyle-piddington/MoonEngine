#include "BasicLoader.h"
#include "thirdparty/tiny_obj_loader.h"
#include "Util/Logger.h"
#include "GLWrapper/GLConstants.h"

using namespace MoonEngine;
using namespace tinyobj;

/** Loads a mesh into associated data structures
 * Loading from file and placing into structures
 * Should really be two separate things
 */
bool BasicLoader::LoadIntoBuffer(std::string fileName,
    GLBuffer * vertexBuffer,
    GLBuffer * indexBuffer,
    GLVertexArrayObject * vertexArrayObject,
    MeshInfo * outInfo,
    bool smooth)
{
    attrib_t attributes;
    std::vector<shape_t> shapes;
    std::vector<material_t> materials;
    //Use TinyOBJLoader to load the object
    std::string err;
    bool success = LoadObj(&attributes, &shapes, &materials, &err, fileName.c_str());
    //@TODO: Load more than one mesh./
    std::vector<glm::vec3> vertPositions;

    if (success)
    {
        //Weave the attributes together
        std::vector<float> dataBuffer = std::vector<float>();
        dataBuffer.reserve(attributes.vertices.size() + attributes.normals.size() + attributes.texcoords.size());
        std::vector<unsigned short> indices;
        bool hasNors = attributes.normals.size() > 0;
        bool hasTexCoords = attributes.texcoords.size() > 0;

        int numVerts;
        if (smooth == false)
        {
            numVerts = loadFlatShade(&dataBuffer, &indices, attributes, shapes);
        }
        else
        {
            numVerts = loadSmoothShade(&dataBuffer, &indices, attributes, shapes);
        }
        //Create BBox
        for (size_t i = 0; i < attributes.vertices.size() / 3; i++)
        {
            vertPositions.push_back(glm::vec3(
                attributes.vertices[i * 3],
                attributes.vertices[i * 3 + 1],
                attributes.vertices[i * 3 + 2])
            );
        }

        vertexBuffer->setData(sizeof(float) * dataBuffer.size(), &(dataBuffer[0]), GL_STATIC_DRAW);
        //For now, indices are all shoved into the same
        //array, instead of multiple parts.
        //Assumption: Normal and texCoord indices are
        //not different than vertex index indices
        // for(const shape_t & shape : shapes)
        // {
        // 	for(const index_t & index : shape.mesh.indices )
        // 	{
        // 		indices.push_back((unsigned short)index.vertex_index);
        // 	}
        // }
        indexBuffer->setData(sizeof(unsigned short) * indices.size(), &(indices[0]), GL_STATIC_DRAW);

        unsigned dataSize = sizeof(float) * 3;
        intptr_t texCoordsOffset = sizeof(float) * 3;

        if (hasNors)
        {
            dataSize += sizeof(float) * 3;
        }
        if (hasTexCoords)
        {
            dataSize += sizeof(float) * 2;
        }

        vertexArrayObject->bindVertexBuffer(
            GL_VERTEX_POSITION_ATTRIBUTE,
            *vertexBuffer,
            3,
            GL_FLOAT,
            false,
            dataSize
        );

        if (hasNors)
        {
            LOG(INFO, "Loading mesh with normals");
            texCoordsOffset += sizeof(float) * 3;
            vertexArrayObject->bindVertexBuffer(
                GL_VERTEX_NORMAL_ATTRIBUTE,
                *vertexBuffer,
                3,
                GL_FLOAT,
                false,
                dataSize,
                (GLvoid *) (sizeof(float) * 3)
            );
        }

        if (hasTexCoords)
        {
            LOG(INFO, "Loading mesh with textures");
            vertexArrayObject->bindVertexBuffer(
                GL_VERTEX_TEXTURE_ATTRIBUTE,
                *vertexBuffer,
                3,
                GL_FLOAT,
                false,
                dataSize,
                (GLvoid *) texCoordsOffset
            );
        }
        vertexArrayObject->bindElementBuffer(*indexBuffer);
        outInfo->numVerts = attributes.vertices.size();
        outInfo->numTris = indices.size();
        outInfo->baseVertex = 0;
        outInfo->indexDataOffset = nullptr;
        outInfo->boundingBox = BoundingBox::BoundPoints(vertPositions);
        outInfo->setVertexArrayObject(vertexArrayObject);
    }
    else
    {
        LOG(ERROR, "Could not load mesh named " + fileName);
    }
    return success;
}

unsigned BasicLoader::loadFlatShade(std::vector<float> * dataBuffer, std::vector<unsigned short> * indexData,
    const attrib_t & attrib, const std::vector<shape_t> & shapes)
{
    int vertIdx = 0;
    bool hasNors = attrib.normals.size() > 0;
    bool hasTexCoords = attrib.texcoords.size() > 0;

    for (const shape_t & shape : shapes)
    {

        size_t index_offset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)
        {
            int fv = shape.mesh.num_face_vertices[f];

            // Loop over vertices in the face.
            for (auto v = 0; v < fv; v++)
            {
                // access to vertex
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                float vx = attrib.vertices[3 * idx.vertex_index + 0];
                float vy = attrib.vertices[3 * idx.vertex_index + 1];
                float vz = attrib.vertices[3 * idx.vertex_index + 2];
                dataBuffer->push_back(vx);
                dataBuffer->push_back(vy);
                dataBuffer->push_back(vz);
                if (hasNors)
                {
                    float nx = attrib.normals[3 * idx.normal_index + 0];
                    float ny = attrib.normals[3 * idx.normal_index + 1];
                    float nz = attrib.normals[3 * idx.normal_index + 2];
                    dataBuffer->push_back(nx);
                    dataBuffer->push_back(ny);
                    dataBuffer->push_back(nz);
                }
                if (hasTexCoords)
                {
                    float tx = attrib.texcoords[2 * idx.texcoord_index + 0];
                    float ty = attrib.texcoords[2 * idx.texcoord_index + 1];
                    dataBuffer->push_back(tx);
                    dataBuffer->push_back(ty);
                }
                indexData->push_back(vertIdx++);
            }
            index_offset += fv;
        }
    }
    return vertIdx;
}

unsigned BasicLoader::loadSmoothShade(std::vector<float> * dataBuffer, std::vector<unsigned short> * indexData,
    const attrib_t & attrib, const std::vector<shape_t> & shapes)
{
    int vertIdx = 0;
    bool hasNors = attrib.normals.size() > 0;
    bool hasTexCoords = attrib.texcoords.size() > 0;
    std::vector<float> vertexPositions = attrib.vertices;
    std::vector<float> vertexNors;

    if (hasNors)
    {
        vertexNors.resize(vertexPositions.size());
    }

    std::vector<float> vertexTex;
    if (hasTexCoords)
    {
        vertexTex = attrib.texcoords;
    }
    std::vector<unsigned> indices;
    vertexNors.resize(attrib.vertices.size());
    for (const shape_t & shape : shapes)
    {

        size_t index_offset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)
        {
            int fv = shape.mesh.num_face_vertices[f];

            // Loop over vertices in the face.
            for (auto v = 0; v < fv; v++)
            {
                // access to vertex
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                vertexPositions[3 * idx.vertex_index + 0] = attrib.vertices[3 * idx.vertex_index + 0];
                vertexPositions[3 * idx.vertex_index + 1] = attrib.vertices[3 * idx.vertex_index + 1];
                vertexPositions[3 * idx.vertex_index + 2] = attrib.vertices[3 * idx.vertex_index + 2];

                vertexNors[3 * idx.vertex_index + 0] += attrib.normals[3 * idx.normal_index + 0];
                vertexNors[3 * idx.vertex_index + 1] += attrib.normals[3 * idx.normal_index + 1];
                vertexNors[3 * idx.vertex_index + 2] += attrib.normals[3 * idx.normal_index + 2];
                indexData->push_back(idx.vertex_index);

            }
            index_offset += fv;
        }
    }
    for (size_t i = 0; i < vertexPositions.size() / 3; i++)
    {
        dataBuffer->push_back(vertexPositions[i * 3 + 0]);
        dataBuffer->push_back(vertexPositions[i * 3 + 1]);
        dataBuffer->push_back(vertexPositions[i * 3 + 2]);
        if (hasNors)
        {
            float nx = vertexNors[i * 3 + 0];
            float ny = vertexNors[i * 3 + 1];
            float nz = vertexNors[i * 3 + 2];
            float len = sqrtf(nx * nx + ny * ny + nz * nz);
            dataBuffer->push_back(nx / len);
            dataBuffer->push_back(ny / len);
            dataBuffer->push_back(nz / len);
        }
        if (hasTexCoords)
        {
            dataBuffer->push_back(vertexTex[i * 2 + 0]);
            dataBuffer->push_back(vertexTex[i * 2 + 1]);
        }
    }
    return vertIdx;
}