#pragma once

#include "Geometry/Transform.h"
#include <unordered_map>
#include <vector>
#include "Loaders/AssimpModelInfo.h"
#include "Component/Component.h"
/**
 * A Skeleton for a model, used in animation.
 */

namespace MoonEngine
{
/**
 * A bone in the skeleton. Holds information regardint it's parent, and it's current transformations.
 */
    class Bone
    {
    private:

        friend class Skeleton;
        
        //Initial offset from the base model
        glm::mat4 offsetTransform;
        //Local animated offset, used in final animation
        glm::mat4 localAnimTransform;
        //Final animation offset, after a heiearchial transformation
        glm::mat4 animTransform;
        //Current name of the bone
        std::string name;
        //Index of the bone in the skeleton
        int idx;


        /**
         * Construct a new bone
         */
        Bone(std::string name, int boneIdx, glm::mat4 offsetTransform);


        /**
         * Update the parent index
         * @param parentBoneIdx index of the parent in the Skeleton 'bone' array.
         */
        void setParent(int parentBoneIdx);

    public:
        /**
         * Set the transform of this bone
         * @param animTransform the new transform as a matrix
         */
        void setAnimatedTransform(glm::mat4 animTransform);

        /**
         * Get the index of this bone in the skeleton's bone array
         * @return the index in 'bone'
         */
        int getIndex()
        {
            return idx;
        }

        /**
         * Get the offset matrix of this bone (relative to it's parent)
         * @return The offset matrix
         */
        glm::mat4 getOffsetMatrix()
        {
            return offsetTransform;
        }

        /**
         * Get the animation matrix of this bone, relative to the world
         * @return the animated final matrix
         */
        glm::mat4 getAnimMatrix()
        {
            return animTransform;
        }


        /**
         * Get the bone name
         * @return the bone's name
         */
        std::string getName()
        {
            return name;
        }


    };

/**
 * Skeleton is a collection of bones organized into a heiearchy
 */
    class Skeleton : public Component
    {
        /**
         * A boneTreeNode is a node in the bone heiarchy
         * Instead of a reference to the bone, the tree node contains the index of the bone in the flat array
         */
        struct BoneTreeNode
        {
            int boneIdx;
            std::vector<BoneTreeNode> children;
        };

        /**
         * The current bones in the system
         */
        std::vector<Bone> bones;

        /**
         * The root of the bone heiearchy
         */
        BoneTreeNode boneRoot;
        /**
         * The root inverse matrix from the import. (Not currently used)
         */
        glm::mat4 rootInverseTransform;
        /**
         * String to index map of the bones
         */
        std::unordered_map<std::string, int> boneMap;

     
        /**
         * Recursivly import bones from an assimp heiarchy
         * @param node   The aiNode to process for bones
         * @param parent The previous node
         */
        void importBonesFromAssimp(AssimpBoneInfo & node, AssimpModelInfo & info, BoneTreeNode & parent);

        /**
         * Pre-multiply the bone animation heiarchy
         * @param node      The node to finalize
         * @param parentMtx the parent matrix to transform the node by
         */
        void finalizeAnimation(BoneTreeNode & node, glm::mat4 parentMtx);

    public:
        /**
         * Construct an empty skeleton
         */
        Skeleton();

        Skeleton(AssimpModelInfo & info);

        ~Skeleton();

        /**
         * Retrive a bone from the tree
         * @param  boneName The name of the bone to lookup
         * @return          A pointer to the bone, or null if it doesn't exist.
         */
        Bone * const getBone(std::string boneName);

        /**
         * Import bones from an assimp data structure
         * @param node the node to process
         */
        void importBonesFromAssimp(AssimpModelInfo & info);

        /**
         * Add a bone to the skeleton
         * @param  boneName name of the bone
         * @param  boneMtx  offset matrix
         * @return          Index
         */
        int addBone(AssimpBoneInfo & info);

        /**
         * Get the size of the bone array
         * @return the number of bones in the skeleton, including roots.
         */
        int getNumBones();

        /**
         * Commit all parent->child relationshps and pre-multiply the bind matricies.
         * Animation matricies will require a re-multiplication every update.
         */
        void finalize();


        /**
         * Update all animated transforms
         * (This should be replaced with a dirty flag later.)
         */
        void finalizeAnimation();


        virtual std::shared_ptr<Component> clone() const;



        /**
         * Get access to all of the bones in the skeleton.
         * (This should be refactored if possible)
         */
        std::vector<Bone> & getAllBones()
        {
            return bones;
        }



    };
};