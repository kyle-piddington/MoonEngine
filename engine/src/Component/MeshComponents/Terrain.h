#pragma once
#include "Geometry/Terrain/CDLODQuadtree.h"
#include "Component/CameraComponents/Camera.h"
#include "Component/MaterialComponents/Material.h"
#include "Mesh.h"
#include "Geometry/MeshCreator.h"
#include "Geometry/Hit.h"
namespace MoonEngine
{
	class Terrain : public Mesh
	{
	public:
	/**
	 * Create a new terrain
	 */
		Terrain(CDLODQuadtree::CreateInfo info, int tileVerts = 9, int maxRenderedTiles = 4096);
		~Terrain();
		Terrain(const Terrain & other);

		void start() override;

		void update(float dt) override;
	/**
	 * Retrieve information associated with the terrain's mesh
	 * @return [description]
	 */
		void  bind() override;
	/**
	 * Retrieve a obundingBox for the entire terrain
	 * @return boundingBox
	 */
		virtual const BoundingBox & getBoundingBox() override;

		virtual const BoundingBox & getExtents() override;
	/**
	 * Clone the component
	 */
		virtual std::shared_ptr<Component> clone() const override;
	/**
	 * Draw the terrain.
	 */
		void draw() const override;

		bool intersectsRay(glm::vec3 origin, glm::vec3 direction, Hit * hit);

		BoundingBox getBoxForCDLODNode(CDLODQuadtree::SelectedNode & node) const;

		float heightAt(float X, float Z);

		glm::vec3 normalAt(float X, float Z);
		//Get the last terrain selection (Used by grass)
		int getLastSelection(CDLODQuadtree::SelectedNode ** bfrPtr);
	private:

		void setupMainUniforms(GLProgram * program) const;
		void setupUniformsForNode(CDLODQuadtree::SelectedNode & node, GLProgram * program, int & lastLOD) const;
		void drawDebugNode(CDLODQuadtree::SelectedNode node) const;
		int maxRenderedTiles;
		CDLODQuadtree::SelectedNode * selectionBuffer;
		Material * terrainMaterial;
		Camera * mainCamera;
		CDLODQuadtree tree;
		glm::vec4 planes[6];
		CDLODQuadtree::CreateInfo creationInfo;
		CDLODQuadtree::LODSelection currentSelection;
		GridInfo gridInfo;
		BoundingBox fullBoundingBox;
		float rasterSizeX;
		float rasterSizeZ;
		unsigned char * normalDataBuffer;

		bool _showAOMap;

		CDLODQuadtree::LODHitInfo lastHitInfo;

		//Some bullshit debugging
		BasicMeshInfo * debugMesh;
	};	
}
