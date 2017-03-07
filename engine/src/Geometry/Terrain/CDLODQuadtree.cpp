#include "CDLODQuadtree.h"
#include "Util/Logger.h"
#include <algorithm>
#include "util/MathUtil.h"
#include <glm/gtc/type_ptr.hpp>
#include "util/MathUtil.h"
using namespace MoonEngine;
CDLODQuadtree::CDLODQuadtree():
m_allNodesBuffer(nullptr),
m_topLevelNodes(nullptr)
{

}

CDLODQuadtree::~CDLODQuadtree()
{
	Clean();
}

bool CDLODQuadtree::Create(const CreateInfo & info)
{
	Clean();
	m_createInfo = info;
	m_rasterSizeX = info.source->getSizeX();
	m_rasterSizeZ = info.source->getSizeZ();
	//If texture too big
	if(m_rasterSizeX > 65535 || m_rasterSizeZ > 65535)
	{
		assert(false);
		return false;
	}
	if(info.LODLevelCount > c_maxLODLevels)
	{
		assert(false);
		return false;
	}
	//Determine size of leaves.
	//(Determine size of one node in the world size. leafNodeSize is biggest node size
	m_leafNodeWorldSizeX = info.leafNodeSize * info.dimensions.size.x / (float)m_rasterSizeX;
	m_leafNodeWorldSizeZ = info.leafNodeSize * info.dimensions.size.z /  (float)m_rasterSizeZ;

	//Get diagonal size for top level.
	m_LODLevelNodDiagSizes[0] = sqrtf(m_leafNodeWorldSizeX * m_leafNodeWorldSizeX + m_leafNodeWorldSizeZ * m_leafNodeWorldSizeZ);

	//Get number of nodes total
	int totalNodes = 0;
	m_topNodeSize =info.leafNodeSize;
	//Start at the bottom LOD level, and calculate
	//number of nodes required per level.
	for(int i = 0; i < info.LODLevelCount; i++)
	{
		if( i != 0)
		{
			m_topNodeSize *= 2;
			m_LODLevelNodDiagSizes[i] = 2*m_LODLevelNodDiagSizes[i-1];

		}
		//Calculate number of nodes for this level.
		////Logic behind this: rasterSizeX / nodeSize tells us how many
		///nodes are needed to cover the layer. We use a ceiling equation to
		///round up. 
		int nodeCountX = (m_rasterSizeX - 1) / m_topNodeSize + 1;
		int nodeCountZ = (m_rasterSizeZ - 1) / m_topNodeSize + 1;
		totalNodes += nodeCountX * nodeCountZ;
	}

	m_allNodesBuffer = new Node[totalNodes];
	int nodeCounter = 0;
	//Create all nodes ahead of time
	////Allocate top level nodes.
	m_topNodeCountX = (m_rasterSizeX - 1) / m_topNodeSize + 1;
	m_topNodeCountZ = (m_rasterSizeZ - 1) / m_topNodeSize + 1;
	//Create slots for 2d top level node array.
	m_topLevelNodes = new Node**[m_topNodeCountZ];
	LOG(GAME, "Top level node size: " + std::to_string(m_topNodeSize));
	for(int  z = 0; z < m_topNodeCountZ; z++)
	{
		m_topLevelNodes[z] = new Node*[m_topNodeCountX];
		for(int x = 0; x < m_topNodeCountX; x++){
			m_topLevelNodes[z][x] = &m_allNodesBuffer[nodeCounter];
			nodeCounter ++;
			//Create this branch of the quadtree.
			
			m_topLevelNodes[z][x]->create(
				x * m_topNodeSize,
				z * m_topNodeSize,
				m_topNodeSize, 
				0, 
				m_createInfo, 
				m_allNodesBuffer, 
				nodeCounter);
		}
	}
	m_allNodesCount = nodeCounter;
	//Logic check, ensure the nodes match up w/ the count.
	assert(nodeCounter == totalNodes);
	int sizeInMemory = totalNodes * sizeof(Node);
	LOG(GAME, "Quadtree created, size in memory: ~" + std::to_string(sizeInMemory/1024.0f) + "Kb");
	return true;
}
/**
 * Initialize a node, and it's children.
 */
void CDLODQuadtree::Node::create(int x, int z, int size, int level, const CreateInfo & createInfo, Node * allNodesBuffer, int &allNodesBufferLastIdx)
{
	//Initialize node members.
	this->X     = (unsigned short)x;       assert( x >= 0 && x < 65535 );
	this->Z    =  (unsigned short)z;        assert( z >= 0 && z < 65535 );
	this->level = (unsigned short)level;   assert( level >= 0 && level <= c_maxLODLevels );
	this->Size  = (unsigned short)size;    assert( size >= 0 && size < 32768 );
	this->minY = 65534;
	this->maxY = 0;
	int rasterSizeX = createInfo.source->getSizeX();
	int rasterSizeZ = createInfo.source->getSizeZ();

	this->SubTL = nullptr;
	this->SubTR = nullptr;
	this->SubBL = nullptr;
	this->SubBR = nullptr;
	

    //At bottom of tree?
	if( size == createInfo.leafNodeSize)
	{
    	//If our size constraint matches the leaf size, we should
    	//be at the bottom.
		assert(level == (createInfo.LODLevelCount - 1));
    	//Tag the leaf level with the leaf tag.
		this->level |= LEAF_FLAG;

    	//Determine the height of the patch
		int limitX = std::min(rasterSizeX, X + size + 1);
		int limitZ = std::min(rasterSizeZ, Z + size + 1);
		createInfo.source->getAreaMinMaxY(X,Z, limitX-X, limitZ-Z,this->minY, this->maxY );
		//LOG(GAME, "Size: " + std::to_string(minY) + " , " + std::to_string(maxY));
		//Retreive heights and 
		//reuse node pointers for ray casting
		//in to node.
		{
			float * pTLY = (float *)&this->SubTL;
			float * pTRY = (float *)&this->SubTR;
			float * pBLY = (float *)&this->SubBL;
			float * pBRY = (float *)&this->SubBR;

			int limitX = std::min( rasterSizeX - 1, X + size );
			int limitZ = std::min( rasterSizeZ - 1, Z + size );
			*pTLY = createInfo.dimensions.minCoords.y + createInfo.source->getHeightAt( X, Z ) * createInfo.dimensions.size.y/ 65535.0f;
			*pTRY = createInfo.dimensions.minCoords.y + createInfo.source->getHeightAt( limitX, Z ) * createInfo.dimensions.size.y/ 65535.0f;
			*pBLY = createInfo.dimensions.minCoords.y + createInfo.source->getHeightAt( X, limitZ) * createInfo.dimensions.size.y/ 65535.0f;
			*pBRY = createInfo.dimensions.minCoords.y + createInfo.source->getHeightAt( limitX, limitZ ) * createInfo.dimensions.size.y/ 65535.0f;

		}
	}
	//Otherwise, create children.
	else
	{
		int subSize = size/2;

		this->SubTL = &allNodesBuffer[allNodesBufferLastIdx++];
		this->SubTL->create(X,Z,subSize,level+1,createInfo,allNodesBuffer,allNodesBufferLastIdx);
		this->minY = std::min(this->minY, this->SubTL->minY);
		this->maxY = std::max(this->maxY, this->SubTL->maxY);
		//Create top right child
		if( (X + subSize) < rasterSizeX)
		{
			this->SubTR = &allNodesBuffer[allNodesBufferLastIdx++];
			this->SubTR->create(X + subSize,Z,subSize,level+1,createInfo,allNodesBuffer,allNodesBufferLastIdx);
			this->minY = std::min(this->minY, this->SubTR->minY);
			this->maxY = std::max(this->maxY, this->SubTR->maxY);
		}
		//Create bottom left child
		if( (Z + subSize) < rasterSizeZ)
		{
			this->SubBL = &allNodesBuffer[allNodesBufferLastIdx++];
			this->SubBL->create(X,Z + subSize,subSize,level+1,createInfo,allNodesBuffer,allNodesBufferLastIdx);
			this->minY = std::min(this->minY, this->SubBL->minY);
			this->maxY = std::max(this->maxY, this->SubBL->maxY);
		}
		//Bottom right case.
		if(( (X + subSize) < rasterSizeX) && ((Z + subSize) < rasterSizeZ))
		{
			this->SubBR = &allNodesBuffer[allNodesBufferLastIdx++];
			this->SubBR->create(X + subSize,Z + subSize, subSize,level+1,createInfo,allNodesBuffer,allNodesBufferLastIdx);
			this->minY = std::min(this->minY, this->SubBR->minY);
			this->maxY = std::max(this->maxY, this->SubBR->maxY);
		}
	}
}

//Selection Routine for CDLODQuadTree
//If the parent is fully in the frustrum, we can return true without vfc
CDLODQuadtree::Node::LODSelectResult CDLODQuadtree::Node::LODSelect(LODSelectInfo & selInfo, bool parentFullyInFrustrum)
{
	BoundingBox box;
	getAABB(box, selInfo.rasterSizeX, selInfo.rasterSizeZ, selInfo.mapDims);

	const glm::vec4 * planes = selInfo.selectionObj->m_frustrumPlanes;
	const glm::vec3 & observerPos = selInfo.selectionObj->m_observerPos;
	const int maxSelectionCount = selInfo.selectionObj->m_maxSelectionCount;
	float * lodRanges = selInfo.selectionObj->m_visibilityRanges;


	IntersectType frustrumIt = (parentFullyInFrustrum) ? (IT_Inside) : box.testFrustrum(planes);
	if(frustrumIt == IT_Outside)
	{

		previouslySelected = false;
		return IT_OutOfFrustrum;
	}
	
	float distLimit = lodRanges[this->getLevel()];
	if(!box.intersectSphereSq(observerPos, distLimit*distLimit))
	{
		previouslySelected = false;
		return IT_OutOfRange;
	}

	LODSelectResult SubTLSelRes = IT_Undefined;
	LODSelectResult SubTRSelRes = IT_Undefined;
	LODSelectResult SubBLSelRes = IT_Undefined;
	LODSelectResult SubBRSelRes = IT_Undefined;
	//Are subchildren in frustrum?
	if(this->getLevel() != selInfo.stopAtLevel)
	{
		
		float nextDistLimit = lodRanges[this->getLevel() + 1];
		if(box.intersectSphereSq(observerPos, nextDistLimit*nextDistLimit))
		{
			bool containedInFrustrum = frustrumIt == IT_Inside;
			//Test children:
			if( SubTL != nullptr ) SubTLSelRes = this->SubTL->LODSelect( selInfo, containedInFrustrum );
			if( SubTR != nullptr ) SubTRSelRes = this->SubTR->LODSelect( selInfo, containedInFrustrum );
			if( SubBL != nullptr ) SubBLSelRes = this->SubBL->LODSelect( selInfo, containedInFrustrum );
			if( SubBR != nullptr ) SubBRSelRes = this->SubBR->LODSelect( selInfo, containedInFrustrum );
		}
	}
	//From CDLOD:
	// We don't want to select sub nodes that are invisible (out of frustum) or are selected;
    // (we DO want to select if they are out of range, since we are not
	bool bRemoveSubTL = (SubTLSelRes == IT_OutOfFrustrum) || (SubTLSelRes == IT_Selected);
	bool bRemoveSubTR = (SubTRSelRes == IT_OutOfFrustrum) || (SubTRSelRes == IT_Selected);
	bool bRemoveSubBL = (SubBLSelRes == IT_OutOfFrustrum) || (SubBLSelRes == IT_Selected);
	bool bRemoveSubBR = (SubBRSelRes == IT_OutOfFrustrum) || (SubBRSelRes == IT_Selected);

	assert( selInfo.selectionCount < maxSelectionCount);
	//If none  of our children were selected, and we still have room to add ourselves:
	if(!(bRemoveSubTL && bRemoveSubTR && bRemoveSubBL && bRemoveSubBR) && (selInfo.selectionCount < maxSelectionCount))
	{
		//Get our level
		int LODLevel = selInfo.stopAtLevel - this->getLevel();
		//Add ourselves to the selection buffer,
		selInfo.selectionObj->m_selectionBuffer[selInfo.selectionCount++] = 
		SelectedNode( this, 
			LODLevel, 
			!bRemoveSubTL, 
			!bRemoveSubTR, 
			!bRemoveSubBL, 
			!bRemoveSubBR, previouslySelected);
		previouslySelected = true;

		if(this->getLevel() != 0)
		{
			float maxCamDist = sqrtf(box.maxDistFromPointSq(observerPos));
			float morphStartRange = selInfo.selectionObj->m_morphStart[selInfo.stopAtLevel - this->getLevel() + 1];
			if(maxCamDist > morphStartRange)
			{
				//Let our caller know that our visibility disatnce could not 
				//contain the morph target.
				selInfo.selectionObj->m_visDistTooSmall = true;
			}
		}
		return IT_Selected;
	}
	if( (SubTLSelRes == IT_Selected) || (SubTRSelRes == IT_Selected) || (SubBLSelRes == IT_Selected) || (SubBRSelRes == IT_Selected) )
	{
		return IT_Selected;
	}
	else
		return IT_OutOfFrustrum;
}

/**
 * Fill an array of nodes with the 
 * subnodes of the CDLOD node.
 */
void CDLODQuadtree::Node::fillSubNodes( Node * nodes[4], int & count ) const
{
	count = 0;

	if( this->isLeaf() )
		return;

	nodes[count++] = this->SubTL;

	if( this->SubTR != nullptr )
		nodes[count++] = this->SubTR;
	if( this->SubBL != nullptr )
		nodes[count++] = this->SubBL;
	if( this->SubBR != nullptr )
		nodes[count++] = this->SubBR;

}

//
// Ray-Triangle Intersection Test Routines          //
// Different optimizations of my and Ben Trumbore's //
// code from journals of graphics tools (JGT)       //
// http://www.acm.org/jgt/                          //
// by Tomas Moller, May 2000                        //
//


bool AABBRay( const glm::vec3 & rayOrigin, const glm::vec3 & rayDirection, const BoundingBox & box, float & distance )
{
      float tmin = -FLT_MAX;        // set to -FLT_MAX to get first hit on line
      float tmax = FLT_MAX;		   // set to max distance ray can travel

      const glm::vec3 _rayOrigin     = rayOrigin;
      const glm::vec3 _rayDirection   = rayDirection;
      const glm::vec3 _min           = box.min();
      const glm::vec3 _max 	= 		   box.max();

      const float EPSILON = 1e-5f;

      for (int i = 0; i < 3; i++) 
      {
      	if ( fabsf(_rayDirection[i]) < EPSILON) 
      	{
            // Parallel to the plane
      		if( _rayOrigin[i] < _min[i] || _rayOrigin[i] > _max[i] )
      		{
               //assert( !k );
               //hits1_false++;
      			return false;
      		}
      	} 
      	else 
      	{
      		float ood = 1.0f / _rayDirection[i];
      		float t1 = (_min[i] - _rayOrigin[i]) * ood;
      		float t2 = (_max[i] - _rayOrigin[i]) * ood;

      		if (t1 > t2) std::swap( t1, t2 );

      		if (t1 > tmin) tmin = t1;
      		if (t2 < tmax) tmax = t2;

      		if (tmin > tmax)
      		{
               //assert( !k );
               //hits1_false++;
      			return false;
      		}

      	}
      }

      distance = tmin;

      return true;
  }

  bool CDLODQuadtree::Node::IntersectRay( 
  	const glm::vec3 & rayOrigin, 
  	const glm::vec3 & rayDirection, 
  	float maxDistance, 
  	LODHitInfo & hitInfo, 
  	const CDLODQuadtree & quadTree ) const
  {
  	BoundingBox boundingBox;
  	getAABB( boundingBox, quadTree.m_rasterSizeX, quadTree.m_rasterSizeZ, quadTree.m_createInfo.dimensions );
  	float hitDistance;
  	if( !AABBRay(rayOrigin, rayDirection, boundingBox, hitDistance ) )
  		return false;

  	if( hitDistance > maxDistance )
  		return false;


  	if(this->isLeaf())
  	{
  		LOG(GAME, "OMG a leaf");

	    // This is the place to place your custom heightmap subsection ray intersection.
	    // Area that needs to be tested lays between this node's [X, Y] and [X + Size, Y + Size] in
	    // the source heightmap coordinates.
	    // Following is the simple two-triangle per leaf-quad approximation.

  		float * pTLY = (float *)&this->SubTL;
  		float * pTRY = (float *)&this->SubTR;
  		float * pBLY = (float *)&this->SubBL;
  		float * pBRY = (float *)&this->SubBR;
  		glm::vec3 bbMin = boundingBox.min();
  		glm::vec3 bbMax = boundingBox.max();
  		glm::vec3 tl( bbMin.x, *pTLY, bbMin.z);
  		glm::vec3 tr( bbMax.x, *pTRY, bbMin.z);
  		glm::vec3 bl( bbMin.x, *pBLY, bbMax.z);
  		glm::vec3 br( bbMax.x, *pBRY, bbMax.z);

  		float u0, v0, dist0;
  		float u1, v1, dist1;
  		bool t0 = MathUtil::IntersectTri( rayOrigin, rayDirection, tl, tr, bl, u0, v0, dist0 );
  		bool t1 = MathUtil::IntersectTri( rayOrigin, rayDirection, tr, bl, br, u1, v1, dist1 );
  		if( t0 && (dist0 > maxDistance) ) t0 = false;
  		if( t1 && (dist1 > maxDistance) ) t1 = false;

  		if( !t0 && !t1 )
  			return false;

  		if( (t0 && !t1) || ((t0 && t1) && (dist0 < dist1)) )
  		{

  			hitInfo.hit = true;
  			hitInfo.position = rayOrigin + rayDirection * dist0;
  			hitInfo.normal = glm::normalize(glm::cross(bl - tr, br - tr));
  			hitInfo.hitNode = this;
  			LOG(ERROR, "OMG A HIT");
  			return true;
  		}

  		hitInfo.position = rayOrigin + rayDirection * dist1;
  		hitInfo.normal = glm::normalize(glm::cross(br-tl, bl-tl));
  		hitInfo.hit = true;
  		hitInfo.hitNode = this;
  		LOG(ERROR, "OMG A HIT");
  		return true;
  	}

  	else
  	{
  		Node * subNodes[4];
  		int subNodeCount;
  		fillSubNodes( subNodes, subNodeCount );

  		float       closestHitDist = FLT_MAX;
  		LODHitInfo closestHit;

  		for( int i = 0; i < subNodeCount; i++ )
  		{
  			LODHitInfo tmpinfo;
  			if( subNodes[i]->IntersectRay( rayOrigin, rayDirection, maxDistance, tmpinfo, quadTree ) )
  			{
  				glm::vec3 diff = tmpinfo.position - rayOrigin;
  				float dist = glm::length(diff);
  				assert( dist <= maxDistance );
  				if( dist < closestHitDist )
  				{
  					closestHitDist = dist;
  					closestHit = tmpinfo;
  				}
  			}
  		}

  		if( closestHitDist != FLT_MAX )
  		{
  			hitInfo = closestHit;  
  			return true;
  		}
  		return false;
  	}

  }


  void CDLODQuadtree::LODSelect(LODSelection  * selectionObj) 
  {
  	const glm::vec3 & cameraPos = selectionObj->m_observerPos;
  	const float visibilityDistance = selectionObj->m_visibilityDistance;
  	const int layerCount = m_createInfo.LODLevelCount;

  	float LODNear = 0;
  	float LODFar = visibilityDistance;
  	float detailBalance = selectionObj->m_LODDistanceRatio;

  	float total = 0;
  	float currentDetailBalance = 1.0f;

  	selectionObj->m_quadTree = this;
  	selectionObj->m_visDistTooSmall = false;
	//Sanity checks.
  	assert(layerCount <= c_maxLODLevels);

	//Determine total max detail
  	for(int i = 0; i < layerCount; i++)
  	{
  		total += currentDetailBalance;
  		currentDetailBalance *= detailBalance;
  	}

	//Set visibility ranges with a detail balancing 
	float sect = (LODFar - LODNear) / total; //(Detail per meter)
	float prevPos = LODNear;
	currentDetailBalance = 1.0f;
	for(int i = 0; i < layerCount; i++)
	{
		selectionObj->m_visibilityRanges[layerCount - i - 1] = prevPos + sect * currentDetailBalance;
		prevPos = selectionObj->m_visibilityRanges[layerCount - i - 1];
		currentDetailBalance *= detailBalance;
	}
	prevPos = LODNear;
	
	for(int i = 0; i < layerCount; i++)
	{
		int idx = layerCount - i - 1;
		selectionObj->m_morphEnd[i] = selectionObj->m_visibilityRanges[idx];
		selectionObj->m_morphStart[i] = prevPos + (selectionObj->m_morphEnd[i] - prevPos) * selectionObj->m_morphStartRatio;
		prevPos = selectionObj->m_morphStart[i];
	}	
	Node::LODSelectInfo lodSelectInfo;
	lodSelectInfo.rasterSizeX = m_rasterSizeX;
	lodSelectInfo.rasterSizeZ = m_rasterSizeZ;
	lodSelectInfo.mapDims = m_createInfo.dimensions;
	lodSelectInfo.selectionCount = 0;
	lodSelectInfo.selectionObj = selectionObj;
	lodSelectInfo.stopAtLevel = layerCount - 1;

	for(int z = 0; z < m_topNodeCountZ; z++)
	{
		for(int x = 0; x < m_topNodeCountX; x++)
		{
			m_topLevelNodes[z][x]->LODSelect(lodSelectInfo, false);
		}
	}

	/*@TODO: Sort by dist to camera ifi requested
	*/
	selectionObj->m_selectionCount = lodSelectInfo.selectionCount;

}

void CDLODQuadtree::Clean()
{
	if(m_allNodesBuffer != nullptr)
	{
		delete[] m_allNodesBuffer;
	}
	m_allNodesBuffer = nullptr;
	if(m_topLevelNodes != nullptr)
	{
		for(int z = 0; z < m_topNodeCountZ; z++)
		{
			delete  [] m_topLevelNodes[z];

		}
		delete [] m_topLevelNodes;

	}
	m_topLevelNodes = nullptr;	


}


CDLODQuadtree::LODSelection::LODSelection(SelectedNode * selbuffer, int maxSelectionCount, const glm::vec3 & observerPos, float visibilityDistance, glm::vec4 frusturmPlanes[6], float lodDistanceRatio, float morphStartRatio, bool sortByDistance)
{
	m_selectionBuffer = selbuffer;
	m_maxSelectionCount = maxSelectionCount;
	m_observerPos = observerPos;
	m_visibilityDistance = visibilityDistance;
	memcpy (m_frustrumPlanes, frusturmPlanes, sizeof(m_frustrumPlanes));
	m_selectionCount = 0;
	m_visDistTooSmall = false;
	m_quadTree = NULL;
	m_LODDistanceRatio = lodDistanceRatio;
	m_morphStartRatio = morphStartRatio;
	m_minSelectedLODLevel = 0;
	m_maxSelectedLODLevel = 0;
	m_sortByDistance = sortByDistance;
}

void CDLODQuadtree::LODSelection::getMorphConsts( int LODLevel, float consts[4] ) const
{
	float mStart = m_morphStart[LODLevel];
	float mEnd = m_morphEnd[LODLevel];
   //
	const float errorFudge = 0.01f;
	mEnd = MathUtil::lerp( mEnd, mStart, errorFudge );
   //
	consts[0] = mStart;
	consts[1] = 1.0f / (mEnd-mStart);
   //
	consts[2] = mEnd / (mEnd-mStart);
	consts[3] = 1.0f / (mEnd-mStart);
}


CDLODQuadtree::LODHitInfo CDLODQuadtree::IntersectRay( const glm::vec3 & rayOrigin, const glm::vec3 & rayDirection, float maxDistance, LODHitInfo * previousInfo) const
{
	LODHitInfo hitInfo;
	//Cached check, pass back in information to determine whether or not 
	if(previousInfo && previousInfo->hitNode)
	{
		if(previousInfo->hitNode->IntersectRay(rayOrigin,rayDirection,maxDistance,hitInfo, *this))
		{
			if(glm::length(hitInfo.position - rayOrigin) <= maxDistance)
			{
				hitInfo.hit = true;
				return hitInfo;				
			}

		}
	}
	
	float       closestHitDist = FLT_MAX;
	LODHitInfo closestHit;

	for( int z = 0; z < m_topNodeCountZ; z++ )
	{
		for( int x = 0; x < m_topNodeCountX; x++ )
		{	
			LODHitInfo hit;
			if( m_topLevelNodes[z][x]->IntersectRay( rayOrigin, rayDirection, maxDistance, hit, *this ) )
			{
				glm::vec3 diff = hit.position - rayOrigin;
				float dist = glm::length(diff);
				assert( dist <= maxDistance );
				if( dist < closestHitDist )
				{
					closestHitDist = dist;
					closestHit = hit;
				}
			}
		}

		if( closestHitDist != FLT_MAX )
		{
			hitInfo.hit = true;
			hitInfo.position = closestHitDist * rayDirection + rayOrigin;
			return hitInfo;
		}
		hitInfo.hit = false;
		return hitInfo;
	}
	return hitInfo;

}




