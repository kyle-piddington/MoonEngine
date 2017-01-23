#pragma once
#include "Component/Component.h"
#include <glm/glm.hpp>
/**
 * Material contains the basic rendering
 * information needed for rendering a material. In this case,
 * a program to determine how this material should be rendered,
 * and a few backup material properites, such as tint.
 */

/**
 * @TODO: Bind Material with a program, instead of just using
 * tint to color the object.
 */

namespace MoonEngine
{
	class Material : public Component
	{
	public:
		Material(glm::vec3 tint = glm::vec3(0,0,0));
		const glm::vec3 & getTint() const;
		void  setTint(glm::vec3 newTint);
	private:
		
		glm::vec3 _tint;
	};
}