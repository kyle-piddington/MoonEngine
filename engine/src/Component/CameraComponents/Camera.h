#pragma once
#include "Component/Component.h"
#include "glm/glm.hpp"
namespace MoonEngine
{
	class Camera : public Component
	{	
	public:
		Camera(std::shared_ptr<GameObject> object,
			 float fov, 
			 float aspect, 
			 float near,
			 float far);
		/**
		 * Get the current projection matrix for this camera
		 * @return the projection matrix
		 */
		const glm::mat4 & getProjection();

		/**
		 * Get the view matrix for this matrix
		 * @return the view matrix
		 */
		const glm::mat4 & getView();

		/**
		 * Set the field of view of this camera
		 * @param fov the new FOV
		 */
		void setFOV(float fov);

		private:
		float aspect;
		float near;
		float far;
		float fov;
		bool vNeedsUpdate;
		glm::mat4 P;
		glm::mat4 V;

	};
}