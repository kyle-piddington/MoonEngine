#pragma once
/**
 * Re-implementation of CDLOD, provided by 
 * Nicolas Devillard.
 * (Almost word-for-word reference and implementation, see
 * License for legal)
 */
#include "IHeightmapSource.h"
#include "MapDimensions.h"
#include "collision/BoundingBox.h"
#include "collision/IntersectionType.h"
#include <string>
namespace MoonEngine
{
    class CDLODQuadtree
    {
        static const int     c_maxLODLevels    = 15;
       
    public:
        struct Node;
        struct CreateInfo
        {
            IHeightmapSource * source;
            int leafNodeSize;
            int LODLevelCount;
            MapDimensions dimensions;
        };

        struct SelectedNode
        {
            unsigned int X;
            unsigned int Z;
            unsigned short Size;
            unsigned short MinY;
            unsigned short MaxY;


            //Is the Top-left of this node selected
            bool TL;
            //Is the Top-right of this node selected
            bool TR;
            //Is the Bottom-left of this node selected
            bool BL;
            //Is the Bottom-right of this node selected
            bool BR;
            float MinDistToCamera; //Only used if sorting applied
            //Current Level of detail for this node.
            int LODLevel;

            SelectedNode(){};
            SelectedNode(const Node * node, int LODLevel, 
                bool tl, bool tr, bool bl, bool br);
            void    getAABB( BoundingBox & aabb, int rasterSizeX, int rasterSizeY, const MapDimensions & mapDims ) const;
        };

        /**
         * Hit response from a raycast
         */
        class LODHitInfo
        {
        public:
            bool hit;
            glm::vec3 normal;
            glm::vec3 position;
        private:
            friend class CDLODQuadtree;
            const Node * hitNode; //For use of priming the CDLODQuadtree raycaster
        };



        class LODSelection
        {
        private:
            //Friend Accessors for tree classes
            friend class CDLODQuadtree;
            friend struct CDLODQuadtree::Node;
            //Input to LOD select algorithm.
            SelectedNode *      m_selectionBuffer; //Buffer for output nodes
            int                 m_maxSelectionCount; // Max number of nodes allowed in selection
            glm::vec3           m_observerPos; //Position of the observer (camera)
            float               m_visibilityDistance; // Max visible distance;
            glm::vec4           m_frustrumPlanes[6]; //VFC planes.
            float               m_LODDistanceRatio; //??
            float               m_morphStartRatio; //??
            bool                m_sortByDistance; //Should the output array be sorted?
            //output for LOD algorithm.
            const CDLODQuadtree * m_quadTree; //Pointer to the tree struct
            float               m_visibilityRanges [c_maxLODLevels];
            float               m_morphEnd[c_maxLODLevels];
            float               m_morphStart[c_maxLODLevels];
            int                 m_selectionCount; //Number of selected nodes.
            bool                m_visDistTooSmall; //If visibility too small, no nodes will be rendered.
            int                 m_minSelectedLODLevel; // Minimum rendered LOD
            int                 m_maxSelectedLODLevel; //Highest LOD selection.

        public:
            /**
             * Create a new selection
             * @param buffer a buffer to hold on to the nubmer of selected nodes
             * @param maxSelectionCount maximum number of nodes to select
             * @param observerPos position of the camera
             * @param visibilityDistance maximum rendered distance of terrain
             * @param frusturmPlanes 6  planes representing the view frustrum
             * @param LODDistanceRatio  ??
             * @param morphStartRatio   ??
             * @bool sortByDistance order output array by distance from viewer
             */
            LODSelection(
                SelectedNode * buffer, 
                int maxSelectionCount, 
                const glm::vec3 & observerPos,
                float visibilityDistance, 
                glm::vec4 frusturmPlanes[6], 
                float LODDistanceRatio, 
                float morphStartRatio = 0.66f, 
                bool sortByDistance = false);
               
            ~LODSelection(){
                
            };

            /**
             * Retrieve a pointer to the underlying quadtree for this selection
             * @return the quadtree
             */
            const CDLODQuadtree * getQuadTree() const;

            /**
             * Get uniforms needed for the morph 
             */
            void getMorphConsts(int LODlevel, float consts[4]) const ;
            /**
             * Get selected nodes.
             * @return an array of nodes.
             */
            inline const SelectedNode * getSelection() const { return m_selectionBuffer;}
            
            /**
             * Get the number of nodes
             * @return number of nodes in getSelection()
             */
            inline int getSelectionCount() const { return m_selectionCount;}

            float getLODDistanceRatio() const { return m_LODDistanceRatio;}

            const float * GetLODLevelRanges() const{return m_morphEnd;}

            bool isVisDistTooSmall() const {return m_visDistTooSmall;}
        
            int getMinSelectedLevel() const {return m_minSelectedLODLevel;}
            
            int getMaxSelectedLevel() const {return m_maxSelectedLODLevel;}
        };
        /**
         * Stack allocated LOD selection.
         * Pros: Easy to set up
         * Cons: Lots of memory thrashing.
         * 
         */
        template <int maxSelectionCount>
        class LODSelectionOnStack : public LODSelection
        {
            SelectedNode m_selectionBufferOnStack[maxSelectionCount];

            public:
             LODSelectionOnStack(const glm::vec3 & observerPos,
                float visibilityDistance, 
                glm::vec4 frusturmPlanes[6], 
                float LODDistanceRatio, 
                float morphStartRatio = 0.66f, 
                bool sortByDistance = false ):
            LODSelection(
                m_selectionBufferOnStack, 
                maxSelectionCount, 
                observerPos, 
                visibilityDistance, 
                frusturmPlanes,
                LODDistanceRatio,
                morphStartRatio,
                sortByDistance)
            {}
        };
        /**
         * Node in the CDLOD Quadtree.
         */
        struct Node
        {
            const unsigned short LEAF_FLAG = 0x8000;
            friend class CDLODQuadtree;
            //Result of the node's selection
            //status.
            enum LODSelectResult
            {
                IT_Undefined,
                IT_OutOfFrustrum,
                IT_OutOfRange,
                IT_Selected,
            };
            //Selection info for querying the quadtree.
            struct LODSelectInfo
            {
                //Selection objs to write nodes to.
                LODSelection * selectionObj;
                //Number of selections to make
                int            selectionCount;
                //LOD max level.
                int            stopAtLevel;
                //??
                int            rasterSizeX;
                //??
                int            rasterSizeZ;
                //Dimensions of the final map.
                MapDimensions  mapDims; 
            };
            unsigned short X;
            unsigned short Z;
            unsigned short Size;

            unsigned short level; // 0x8000 used to store whether or not node is a leaf.
            unsigned short minY;
            unsigned short maxY;

            //Children node pointers.
            //In CDLOD implementation, reused as float* pointers 
            //(Pointer size == float size)
            Node *  SubTL; //Top leftchild
            Node *  SubTR; //Top Right child
            Node *  SubBL; //Bottom Left Child
            Node *  SubBR; //Bottom right child;
            
            //From CDLOD: 
            // Level 0 is a root node, and level 'LodLevel-1' is a leaf node. So the actual 
            // LOD level equals 'LODLevelCount - 1 - Node::GetLevel()'.
            unsigned short getLevel() const { return (unsigned short) (level & 0x7FFF); } //Mask out leaf node.
            bool           isLeaf()   const { return (level & 0x8000) != 0;}
            /**
             * [getWorldMinMaxX description]
             * @param minX        [description]
             * @param maxX        [description]
             * @param rasterSizeX [description]
             * @param mapDims     [description]
             */
            void getWorldMinMaxX(float & minX, float & maxX, int rasterSizeX, const MapDimensions & mapDims) const;
            void getWorldMinMaxY(float & minY, float & maxY, const MapDimensions & mapDims) const;
            void getWorldMinMaxZ(float & minZ, float & maxZ, int rasterSizeZ, const MapDimensions & mapDims) const;

            void getAABB(BoundingBox & aabb, int rasterSizeX, int rasterSizeZ, const MapDimensions & mDims) const;
            void getBoundingSphere(glm::vec3 & sphereCenter, float & sphereRad, const CDLODQuadtree & quadTree) const;
            void fillSubNodes( Node * nodes[4], int & count ) const;
        private:
            void create(int x, int y, int size, int level, const CreateInfo & CreateInfo, Node * allNodesBuffer, int &allNodesBufferLastIdx);

            LODSelectResult LODSelect(LODSelectInfo & selInfo, bool parentInFrustrum) const;

            void getAreaMinMaxHeight(int fromX, int fromZ, int toX, int toZ, float &minY, float &maxY, const CDLODQuadtree & quadtree) const;
        
            bool IntersectRay( const glm::vec3 & rayOrigin, const glm::vec3 & rayDirection, float maxDistance, LODHitInfo & hitInfo, const CDLODQuadtree & quadTree ) const;

        };
    private:
        CreateInfo m_createInfo;

        Node *      m_allNodesBuffer;
        int         m_allNodesCount;
        //Triple pointer to the starting set of nodes
        Node ***    m_topLevelNodes;

        int m_topNodeSize; //Number of top level nodes;
        int m_topNodeCountX;
        int m_topNodeCountZ;

        int m_leafNodeSize; //Size of the leaves
        //Raster size is the size of a 
        //node
        int m_rasterSizeX; 
        int m_rasterSizeZ;

        float m_leafNodeWorldSizeX;
        float m_leafNodeWorldSizeZ;

        float m_LODLevelNodDiagSizes[c_maxLODLevels]; //Diagonal size info

    public:
        CDLODQuadtree();
        virtual ~CDLODQuadtree();

        bool Create(const CreateInfo & info);
        void Clean();

        int     GetLODLevelCount() const {return m_createInfo.LODLevelCount;}

        void LODSelect(LODSelection * selectionObj) const;

        void getAreaMinMaxHeight(
            float fromX, 
            float fromZ,
            float sizeX, 
            float sizeZ, 
            float &minY, 
            float &maxY) const;

        int getRasterSizeX() const { return  m_rasterSizeX;}
        int getRasterSizeZ() const { return m_rasterSizeZ;}

        float getLODLevelNodeDiagonalSize(int lodLevel) const { 
            assert (lodLevel >= 0 && lodLevel < m_createInfo.LODLevelCount);
            return m_LODLevelNodDiagSizes[lodLevel];
        }

        LODHitInfo IntersectRay( const glm::vec3 & rayOrigin, const glm::vec3 & rayDirection, float maxDistance, LODHitInfo * previousInfo = nullptr ) const;


        const MapDimensions & getWorldMapDims() const { return m_createInfo.dimensions;}
    };

    //Inline Functions
    
    void inline CDLODQuadtree::Node::getBoundingSphere( glm::vec3 & sphereCenter, float & sphereRadiusSq, const CDLODQuadtree & quadTree ) const
    {
       const float scaleX = quadTree.m_createInfo.dimensions.size.x / (float)(quadTree.m_rasterSizeX-1);
       const float scaleY = quadTree.m_createInfo.dimensions.size.y / 65535.0f;
       const float scaleZ = quadTree.m_createInfo.dimensions.size.z / (float)(quadTree.m_rasterSizeZ-1);
       
       float sizeHalfX = this->Size / 2.0f;
       float sizeHalfY = (this->maxY - this->minY) / 2.0f;
       float sizeHalfZ = this->Size / 2.0f;

       const float midX = this->X + sizeHalfX;
       const float midY = this->minY + sizeHalfY;
       const float midZ = this->Z + sizeHalfZ;
       sphereCenter.x = quadTree.m_createInfo.dimensions.minCoords.x + midX * scaleX;
       sphereCenter.y = quadTree.m_createInfo.dimensions.minCoords.y + midY * scaleY;
       sphereCenter.z = quadTree.m_createInfo.dimensions.minCoords.z + midZ * scaleZ;

       sizeHalfX = sizeHalfX * scaleX;
       sizeHalfY = sizeHalfY * scaleY;
       sizeHalfZ = sizeHalfZ * scaleZ;
       //Sphere bounds corner of the box.
       sphereRadiusSq = sizeHalfX*sizeHalfX + sizeHalfY*sizeHalfY + sizeHalfZ * sizeHalfZ;
    }
    //Get the minimum and maximum X values of this node.
    void inline CDLODQuadtree::Node::getWorldMinMaxX(float &minX, float &maxX, int rasterSizeX, const MapDimensions & mapDims) const
    {
        minX = mapDims.minCoords.x + this->X * mapDims.size.x / (float)(rasterSizeX - 1);
        maxX = mapDims.minCoords.x + (this->X  + this->Size)* mapDims.size.x / (float)(rasterSizeX - 1);
    }
    //get the minumum and maximum Z values of this node.
    void inline CDLODQuadtree::Node::getWorldMinMaxZ(float &minZ, float &maxZ, int rasterSizeZ, const MapDimensions & mapDims) const
    {
        minZ = mapDims.minCoords.z + this->Z * mapDims.size.z / (float)(rasterSizeZ - 1);
        maxZ = mapDims.minCoords.z + (this->Z  + this->Size)* mapDims.size.z / (float)(rasterSizeZ - 1);
    }

    void inline CDLODQuadtree::Node::getWorldMinMaxY(float &minY, float &maxY, const MapDimensions & mapDims) const
    {
        minY = mapDims.minCoords.y + this->minY * mapDims.size.y / 65535.0f; //Unsigned short
        maxY = mapDims.minCoords.y + this->maxY * mapDims.size.y / 65535.0f; //Unsigned short      
    }

    void inline CDLODQuadtree::Node::getAABB(BoundingBox & box, int rasterSizeX, int rasterSizeZ, const MapDimensions & mapDims) const
    {
        float minX, maxX, minY, maxY, minZ, maxZ;
        getWorldMinMaxX(minX,maxX,rasterSizeX,mapDims);
        getWorldMinMaxY(minY,maxY,mapDims);
        getWorldMinMaxZ(minZ,maxZ,rasterSizeZ,mapDims);
        box = BoundingBox(minX,maxX,minY,maxY,minZ,maxZ);
		box.xHalfWidth *= 1.2f;
		box.yHalfWidth = 10;
		box.zHalfWidth *= 1.2f;
           
    }

    void inline CDLODQuadtree::SelectedNode::getAABB( BoundingBox & aabb, int rasterSizeX, int rasterSizeZ, const MapDimensions & mapDims ) const
    {
       float minX, maxX, minY, maxY, minZ, maxZ;
       minX = mapDims.minCoords.x + this->X * mapDims.size.x/ (float)(rasterSizeX-1);
       maxX = mapDims.minCoords.x + (this->X+this->Size) * mapDims.size.x/ (float)(rasterSizeX-1);
       minY = mapDims.minCoords.y + this->MinY * mapDims.size.y / 65535.0f;
       maxY = mapDims.minCoords.y + this->MaxY * mapDims.size.y / 65535.0f;

       minZ = mapDims.minCoords.z + this->Z * mapDims.size.z / (float)(rasterSizeZ-1);
       maxZ = mapDims.minCoords.z + (this->Z+this->Size) * mapDims.size.z / (float)(rasterSizeZ-1);
       aabb = BoundingBox(minX,maxX,minY,maxY,minZ,maxZ);
       //aabb.yHalfWidth = std::max(aabb.yHalfWidth, 1e-3f);
    }
    inline CDLODQuadtree::SelectedNode::SelectedNode( const Node * node, int LODLevel, bool tl, bool tr, bool bl, bool br )
   : LODLevel(LODLevel), TL(tl), TR(tr), BL(bl), BR(br)
    {
       this->X = node->X;
       this->Z = node->Z;
       this->Size = node->Size;
       this->MinY = node->minY;
       this->MaxY = node->maxY;
    }

}
