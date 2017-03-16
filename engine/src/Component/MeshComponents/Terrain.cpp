#include "Terrain.h"
#include "Geometry/MeshCreator.h"
#include "GlobalFuncs/GlobalFuncs.h"
#include "Util/Logger.h"
#include "GLUtil/GLNormalMapCreator.h"
#include <cassert>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <algorithm>
#include "thirdparty/imgui/imgui.h"
//Debug shit
#include "Libraries/Library.h"
using namespace MoonEngine;
Terrain::Terrain(CDLODQuadtree::CreateInfo info, int tileVerts, int maxRenderedTiles):
creationInfo(info),
maxRenderedTiles(maxRenderedTiles),
mainCamera(nullptr),
currentSelection(CDLODQuadtree::LODSelection(selectionBuffer,maxRenderedTiles,glm::vec3(0,0,0),0,planes,2.f))
{
	selectionBuffer = new CDLODQuadtree::SelectedNode[maxRenderedTiles];
	gridInfo = MeshCreator::CreateGrid(glm::vec2(0),glm::vec2(1),tileVerts);	
	tree.Create(info);
	glm::vec3 worldSize = info.dimensions.size;
	glm::vec3 worldCenter = info.dimensions.minCoords + worldSize/2.0f;
	fullBoundingBox = BoundingBox(worldCenter,worldSize.x,worldSize.y,worldSize.z);
	debugMesh = Library::MeshLib->getInfoForMeshNamed("cube.obj",false);
	lastHitInfo.hit = false;
	rasterSizeX = (float)creationInfo.source->getSizeX();
	rasterSizeZ = creationInfo.source->getSizeZ();


}

Terrain::Terrain(const Terrain & other):
maxRenderedTiles(other.maxRenderedTiles),
selectionBuffer(new CDLODQuadtree::SelectedNode[other.maxRenderedTiles]),
mainCamera(other.mainCamera),
creationInfo(other.creationInfo),
gridInfo(other.gridInfo),
currentSelection(other.currentSelection),
fullBoundingBox(other.fullBoundingBox)
{
	tree.Create(creationInfo);
}

Terrain::~Terrain()
{
	if(selectionBuffer != nullptr)
	{
		delete [] selectionBuffer;
	}
}

void Terrain::start()
{
	//Create Heightmap
	GLNormalMapCreator creator;
	GLTextureConfiguration normalMapCfg(creationInfo.source->getSizeX(), creationInfo.source->getSizeZ());
	auto tex = creator.GenerateNormalMap(creationInfo.source, normalMapCfg,creationInfo.dimensions, &normalDataBuffer);
	//Track the heightmap in the texture library.
	Library::TextureLib->addTexture("heightmap_normal",tex);
	terrainMaterial = gameObject->getComponent<Material>();
	terrainMaterial->addTexture("heightmap_normal",tex.get());
	mainCamera = GetWorld()->findGameObjectWithComponent<Camera>()->getComponent<Camera>();
	assert(mainCamera != nullptr);
	gameObject->addTag(T_Terrain);

}

void Terrain::update(float dt)
{
	
	mainCamera->getPlanes(planes);
	currentSelection = CDLODQuadtree::LODSelection(selectionBuffer, maxRenderedTiles, mainCamera->getGameObject()->getTransform().getPosition(), 
		mainCamera->getFar(),planes,4.0f);

	tree.LODSelect(&currentSelection);

	ImGui::Begin("Terrain rendering");
	{
		ImGui::Checkbox("Terrain AO ", &_showAOMap);

	}
	ImGui::End();
}

const BoundingBox & Terrain::getBoundingBox()
{
	return fullBoundingBox;
}

const BoundingBox & Terrain::getExtents()
{
	return fullBoundingBox;
}

std::shared_ptr<Component> Terrain::clone() const
{
	return std::make_shared<Terrain>(*this);
}

void Terrain::bind() 
{
	gridInfo.meshInfo->bind();
}

//Set uniforms constant throughout the draw call.
void Terrain::setupMainUniforms(GLProgram * program) const
{
	glm::vec3 g_cameraPosition = mainCamera->getGameObject()->getTransform().getPosition();
	
	int textureWidth  = tree.getRasterSizeX();
	int textureHeight = tree.getRasterSizeZ();

	glm::vec4 g_terrainScale = glm::vec4(creationInfo.dimensions.size,0.0);
	glm::vec4 g_terrainOffset = glm::vec4(creationInfo.dimensions.minCoords, 0);
	glm::vec2 g_samplerWorldToTextureScale = glm::vec2( (textureWidth-1.0f) / (float)textureWidth, (textureHeight-1.0f) / (float)textureHeight);
	glm::vec2 g_quadWorldMax = glm::vec2(creationInfo.dimensions.MaxX(),creationInfo.dimensions.MaxZ());
	glm::vec4 g_heightmapTextureInfo = glm::vec4(textureWidth, textureHeight, 1.0f/textureWidth, 1.0f/textureHeight);
	glm::vec3 g_gridDim = glm::vec3(gridInfo.gridSize - 1, (gridInfo.gridSize - 1)/2.0f, 1 / ((gridInfo.gridSize - 1) / 2.0f));
	glUniform3fv(program->getUniformLocation("g_cameraPosition"),1,glm::value_ptr(g_cameraPosition));
	glUniform2fv(program->getUniformLocation("g_samplerWorldToTextureScale"),1,glm::value_ptr(g_samplerWorldToTextureScale));
	glUniform3fv(program->getUniformLocation("g_gridDim"),1,glm::value_ptr(g_gridDim));
	glUniform4fv(program->getUniformLocation("g_heightmapTextureInfo"),1,glm::value_ptr(g_heightmapTextureInfo));
	glUniform4fv(program->getUniformLocation("g_terrainScale"),1,glm::value_ptr(g_terrainScale));
	glUniform4fv(program->getUniformLocation("g_terrainOffset"),1,glm::value_ptr(g_terrainOffset));
	glUniform2fv(program->getUniformLocation("g_quadWorldMax"),1,glm::value_ptr(g_quadWorldMax));
	glUniform2f(program->getUniformLocation("t_resolution"), (float)textureWidth, (float)textureHeight);

	glUniform1i(program->getUniformLocation("showAO"),_showAOMap);

   //V( vertexShader.SetFloatArray( "g_terrainScale", mapDims.SizeX, mapDims.SizeY, mapDims.SizeZ, 0.0f ) );
   //V( vertexShader.SetFloatArray( "g_terrainOffset", mapDims.MinX, mapDims.MinY, mapDims.MinZ, 0.0f ) );
   //V( vertexShader.SetFloatArray( "g_samplerWorldToTextureScale", (textureWidth-1.0f) / (float)textureWidth, (textureHeight-1.0f) / (float)textureHeight ) );
   //V( vertexShader.SetFloatArray( "g_heightmapTextureInfo", (float)textureWidth, (float)textureHeight, 1.0f / (float)textureWidth, 1.0f / (float)textureHeight ) );

}

void Terrain::setupUniformsForNode(CDLODQuadtree::SelectedNode & node, GLProgram * program, int & lastLOD) const
{
	if( lastLOD != node.LODLevel )
	{
		lastLOD = node.LODLevel;
		float morphs[4];
		currentSelection.getMorphConsts( lastLOD, morphs );
		glUniform4fv(program->getUniformLocation("t_param.g_morphConsts"),1,morphs);
	}

	//batchInfo.VertexShader->SetFloatArray( batchInfo.VSMorphConstsHandle, v, 4 );
	BoundingBox boundingBox;
	node.getAABB( boundingBox, tree.getRasterSizeX(), tree.getRasterSizeZ(), creationInfo.dimensions );
	glUniform4f(program->getUniformLocation("t_param.g_quadScale"),boundingBox.xHalfWidth * 2, boundingBox.yHalfWidth * 2, boundingBox.zHalfWidth * 2, node.LODLevel);
	glm::vec4 minCoords = glm::vec4(boundingBox.min(),0);
	glUniform4fv(program->getUniformLocation("t_param.g_quadOffset"),1,glm::value_ptr(minCoords));
}

BoundingBox Terrain::getBoxForCDLODNode(CDLODQuadtree::SelectedNode & node) const
{
	BoundingBox box;
	node.getAABB(box, tree.getRasterSizeX(), tree.getRasterSizeZ(), creationInfo.dimensions);
	return box;
}
void Terrain::drawDebugNode(CDLODQuadtree::SelectedNode node) const
{
	Transform boxTrasnform;
	
	GLProgram * dbg = Library::ProgramLib->getProgramForName("default.program");
	dbg->enable();
	BoundingBox boundingBox;
	node.getAABB( boundingBox, tree.getRasterSizeX(), tree.getRasterSizeZ(), creationInfo.dimensions );

	boxTrasnform.setScale(glm::vec3(boundingBox.xHalfWidth,boundingBox.yHalfWidth, boundingBox.zHalfWidth) * 2.0f);
	boxTrasnform.setPosition(boundingBox.centerPoint);

	glUniformMatrix4fv(dbg->getUniformLocation("P"),1,GL_FALSE,glm::value_ptr(mainCamera->getProjection()));
	glUniformMatrix4fv(dbg->getUniformLocation("V"),1,GL_FALSE,glm::value_ptr(mainCamera->getView()));
	glUniformMatrix4fv(dbg->getUniformLocation("M"),1,GL_FALSE,glm::value_ptr(boxTrasnform.getMatrix()));
	debugMesh->bind();
	glDrawElementsBaseVertex(GL_TRIANGLES,
		debugMesh->numTris,
		GL_UNSIGNED_SHORT,
		debugMesh->indexDataOffset,
		debugMesh->baseVertex);
	gridInfo.meshInfo->bind();
	terrainMaterial->getProgram()->enable();
}


int Terrain::getLastSelection(CDLODQuadtree::SelectedNode ** bfrPtr)
{
	*bfrPtr = selectionBuffer;
	return currentSelection.getSelectionCount();
}

void Terrain::draw() const
{
	Transform t;
	GLProgram * prog = terrainMaterial->getProgram();
	setupMainUniforms(prog);
	int lastLOD = -1;

	BasicMeshInfo * meshInfo = gridInfo.meshInfo;
	//LOG(INFO, "Drawing " + std::to_string(currentSelection.getSelectionCount()) + "nodes");
	for(int idx = 0; idx < currentSelection.getSelectionCount(); idx++)
	{
		CDLODQuadtree::SelectedNode node = selectionBuffer[idx];
		setupUniformsForNode(node,prog, lastLOD);
		//drawDebugNode(node);
		if(node.TL && node.TR && node.BL && node.BR)
		{
			glDrawElementsBaseVertex(GL_TRIANGLES,
				meshInfo->numTris,
				GL_UNSIGNED_SHORT,
				meshInfo->indexDataOffset,
				meshInfo->baseVertex);

		}
		else
		{
			int quarterTris = meshInfo->numTris / 4;
			if(node.TL)
			{
				//LOG(INFO, "Drawing TL");
				glDrawElementsBaseVertex(GL_TRIANGLES,
					quarterTris,
					GL_UNSIGNED_SHORT,
					(GLvoid *)((unsigned short * ) meshInfo->indexDataOffset + gridInfo.tlIdx),
					meshInfo->baseVertex);

			}
			if(node.TR)
			{
				//LOG(INFO, "Drawing TR");
				glDrawElementsBaseVertex(GL_TRIANGLES,
					quarterTris,
					GL_UNSIGNED_SHORT,
					(GLvoid *)((unsigned short * ) meshInfo->indexDataOffset + gridInfo.trIdx),
					meshInfo->baseVertex);

			}
			if(node.BL)
			{
				//LOG(INFO, "Drawing BL");
				glUniform3f(prog->getUniformLocation("tint"),0,0,1);
				glDrawElementsBaseVertex(GL_TRIANGLES,
					quarterTris,
					GL_UNSIGNED_SHORT,
					(GLvoid *)((unsigned short * ) meshInfo->indexDataOffset + gridInfo.blIdx),
					meshInfo->baseVertex);
				
			}
			if(node.BR)
			{	
				glUniform3f(prog->getUniformLocation("tint"),0,0,0);
				glDrawElementsBaseVertex(GL_TRIANGLES,
					quarterTris,
					GL_UNSIGNED_SHORT,
					(GLvoid *)((unsigned short * ) meshInfo->indexDataOffset + gridInfo.brIdx),
					meshInfo->baseVertex);
				
			}
		}
	}
}



bool Terrain::intersectsRay(glm::vec3 origin, glm::vec3 direction, Hit * hit)
{

	CDLODQuadtree::LODHitInfo * cachedInfo = nullptr;
	if(lastHitInfo.hit)
	{
		LOG(GAME, "Using cached node");
		//cachedInfo = &lastHitInfo;
	}
	lastHitInfo = tree.IntersectRay(origin,direction,FLT_MAX, cachedInfo);

	if(lastHitInfo.hit)
	{
		LOG(GAME, "Terrain intersection");
		if(hit != nullptr)
		{
			hit->object = this->gameObject;
			hit->intersectionPoint = lastHitInfo.position;
			hit->intersectionNormal = lastHitInfo.normal;

			hit->distance = glm::length(origin - lastHitInfo.position);
			LOG(GAME, "Hit distance: " + std::to_string(hit->distance));
		}
		return true;
	}
	else
	{
		return false;
	}
}

float Terrain::heightAt(float x, float z)
{
	//Convert world to pixel coordinates.
	glm::vec3 minCoords = creationInfo.dimensions.minCoords;
	glm::vec3 mapScale = creationInfo.dimensions.size;
	float px = (x - minCoords.x)/mapScale.x * (rasterSizeX - 1);
	float pz = (z - minCoords.z)/mapScale.z * (rasterSizeZ - 1);
	px = std::max(std::min(px, rasterSizeX - 1), 0.0f);
	pz = std::max(std::min(pz, rasterSizeZ - 1), 0.0f);
	float sourceHeight = creationInfo.source->getHeightAtFloat(px,pz);
	return minCoords.y + sourceHeight/65535.0f * mapScale.y;

}

glm::vec3 Terrain::normalAt(float x, float z)
{
	glm::vec3 minCoords = creationInfo.dimensions.minCoords;
	glm::vec3 mapScale = creationInfo.dimensions.size;
	int px = (x - minCoords.x)/mapScale.x * (rasterSizeX - 1);
	int pz = (z - minCoords.z)/mapScale.z * (rasterSizeZ - 1);
	px = (int)std::max(std::min(px, (int)(rasterSizeX - 1)), 0);
	pz = (int)std::max(std::min(pz, (int)(rasterSizeZ - 1)), 0);
	int txWidth = creationInfo.source->getSizeX();
	char nx = normalDataBuffer[4*(pz * creationInfo.source->getSizeX() + px)];
	char ny = normalDataBuffer[4*(pz * creationInfo.source->getSizeX() + px) + 1];
	char nz = normalDataBuffer[4*(pz * creationInfo.source->getSizeX() + px) + 2];
	float fx = nx/128.0f - 1.0f;
	float fy = ny/128.0f - 1.0f;
	float fz = nz/128.0f - 1.0f;
	return glm::vec3(fx,fy,fz);

}
