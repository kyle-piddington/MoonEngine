#include "Grass.h"
#include "GlobalFuncs/GlobalFuncs.h"
#include "thirdparty/imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>
using namespace MoonEngine;


Grass::Grass(std::string mesh, bool smooth, unsigned numInstances):
Mesh(),
_maxInstances(numInstances),
_cInstance(0),
_numOfInstances(0),
windTime(0),
bBox(glm::vec3(0,0,0),1000,1000,1000)
{
    _meshInfo = EngineApp::GetAssetLibrary().MeshLib->getInfoForMeshNamed(mesh, smooth);
   _instanceBuffer = std::make_shared<GLBuffer>(GL_ARRAY_BUFFER, sizeof(glm::mat4) * _maxInstances, nullptr, GL_DYNAMIC_DRAW);
    
    _meshInfo->getVAO()->bindVertexBuffer(GL_VERTEX_INSTANCE_MATRIX_ATTRIBUTE, *_instanceBuffer,
        sizeof(glm::vec4), GL_FLOAT, GL_FALSE, sizeof(glm::vec4));
    LOG_GL(__FILE__, __LINE__);
        

}

void Grass::start()
{
    auto tObj = GetWorld()->findGameObjectWithTag(T_Terrain);
    if(tObj != nullptr)
    {
        _terrainComponent = tObj->getComponent<Terrain>();
    }
    player = GetWorld()->findGameObjectWithTag(T_Player);
}

void Grass::update(float dt)
{
    windTime+=dt;
    if(_terrainComponent != nullptr)
    {
        CDLODQuadtree::SelectedNode * nodes;
        int numNodes = _terrainComponent->getLastSelection(&nodes);
        std::vector<glm::mat4> newTransforms = std::vector<glm::mat4>();
        for(int i = 0; i < numNodes; i++)
        {
            if(nodes[i].LODLevel < 3 && !nodes[i].previouslySelected)
            {
                generateGrassForTile(nodes[i], &newTransforms);
            }
        }
        if(newTransforms.size() > 0)
        {
            addTransformsToBuffer(newTransforms);
        }
    }
}

void Grass::bind()
{
    _meshInfo->bind();
}

const BoundingBox & Grass::getBoundingBox()
{
	return bBox;
}

const BoundingBox & Grass::getExtents()
{
    return bBox;
}

std::shared_ptr<Component> Grass::clone() const
{
	return std::make_shared<Grass>(*this);
}

void Grass::generateGrassForTile(CDLODQuadtree::SelectedNode & node, std::vector<glm::mat4> * newTransforms)
{
    BoundingBox box = _terrainComponent->getBoxForCDLODNode(node);
	float grassScale = 0.35*(node.LODLevel);

    
    for(int i = -4; i < 4; i++)
    {
        float u = (i) / 4.0f;
        for(int j = - 4; j < 4; j++)
        {
            float v = (j)/3.0f;
            float jitterX = (rand() / (float)RAND_MAX)/4.0f - 0.5f;
            float jitterY = (rand() / (float)RAND_MAX)/4.0f - 0.5f;
            u+=jitterX;
            v+= jitterY;
            float xPt =  box.centerPoint.x + box.xHalfWidth*u;
            float zPt =  box.centerPoint.z + box.zHalfWidth*v;
            float ht = _terrainComponent->heightAt(xPt,zPt);
            glm::vec3 nor = glm::normalize(_terrainComponent->normalAt(xPt,zPt));
            //LOG(GAME, std::to_string(nor.x)+","+std::to_string(nor.y)+","+ std::to_string(nor.z));
            if(glm::dot(nor,glm::vec3(0,1,0)) < 0.95f)
            {
                continue;
            }
            glm::vec3 c = glm::vec3(xPt,ht,zPt);
            
            float theta = rand()/(float)RAND_MAX;
            
            glm::mat4 t = glm::mat4(
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            c.x,c.y,c.z,1);

            glm::mat4 r = 
            glm::mat4_cast(glm::angleAxis(theta, nor));

            glm::mat4 s = glm::mat4(
                0.15 + 2*grassScale, 0, 0, 0,
                0, 0.05 + grassScale, 0, 0,
                0, 0, 0.15 + 2*grassScale, 0,
                0,0,0, 1
            );

            newTransforms->push_back(t*r*s);
        }
    }
    
}

void Grass::addTransformsToBuffer(const std::vector<glm::mat4> & newTransforms)
{
    LOG(GAME, "Adding new grass");
    LOG_GL(__FILE__, __LINE__);
    _instanceBuffer->bind();
    LOG_GL(__FILE__, __LINE__);
    if(_cInstance + newTransforms.size() > _maxInstances)
    {
        int numTransforms = _maxInstances - _cInstance;
        glBufferSubData(GL_ARRAY_BUFFER,
         _cInstance * sizeof(glm::mat4), 
         numTransforms * sizeof(glm::mat4),
         &(newTransforms[0]));
        LOG_GL(__FILE__, __LINE__);
        //Wrap circular buffer

        glBufferSubData(GL_ARRAY_BUFFER,
         0, 
         (newTransforms.size() - numTransforms) * sizeof(glm::mat4),
         &(newTransforms[numTransforms]));
        LOG_GL(__FILE__, __LINE__);

        _cInstance = (newTransforms.size() - numTransforms);
        _numOfInstances = _maxInstances;

    }
    else
    {
        glBufferSubData(GL_ARRAY_BUFFER,
         _cInstance * sizeof(glm::mat4), 
         newTransforms.size() * sizeof(glm::mat4),
         &(newTransforms[0]));
        _cInstance += newTransforms.size();
        _numOfInstances += newTransforms.size();
        LOG_GL(__FILE__, __LINE__);
    }
    _instanceBuffer->unbind();

}
void Grass::draw() const
{
    static float windSpeed = 1.1f;
    static float windStrenght = 0.25f;
    static float windOsc = 0.75f;
    static float windDir[2] = {1,0.7};
    ImGui::Begin("Grass");
    {
        ImGui::DragFloat("Wind Speed",&windSpeed);
        ImGui::DragFloat("Wind Strength", &windStrenght);
        ImGui::DragFloat("Wind Osc", &windOsc);
        ImGui::DragFloat2("Wind Dir", windDir);
           
    }
    ImGui::End();
    glDisable(GL_CULL_FACE);
    GLProgram * mat = gameObject->getComponent<Material>()->getProgram();
    glUniform1f(mat->getUniformLocation("windSpeed"),windSpeed);
    glUniform1f(mat->getUniformLocation("windStrength"),windStrenght);
    glUniform1f(mat->getUniformLocation("windOsc"),windOsc);
    glUniform1f(mat->getUniformLocation("windTime"),windTime);
    glUniform2fv(mat->getUniformLocation("windDir"),1,windDir);
    if(player != nullptr)
    {
        glm::vec3 playerPos = player->getTransform().getPosition();
        glUniform3fv(mat->getUniformLocation("player"),1,glm::value_ptr(playerPos));

    }
    if(_numOfInstances > 0)
    {

        glDrawElementsInstanced(GL_TRIANGLES,
        _meshInfo->numTris,
        GL_UNSIGNED_SHORT,
        _meshInfo->indexDataOffset,
        _numOfInstances);    
        LOG_GL(__FILE__, __LINE__);       
    }
    glEnable(GL_CULL_FACE);

}