#pragma once
#include "Component/Component.h"
#include "GL/GLProgram.h"
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
		Material(
				 glm::vec3 tint = glm::vec3(0,0,0), std::string programName = "default.program");
		
		/**
		 * retrieve a base tint material used by all
		 * material program.s
		 * @return  the current tint value
		 */
		const glm::vec3 & getTint() const;
		/**
		 * Set the tint of this material
		 * @param  newTint new tint
		 */
		virtual void  setTint(glm::vec3 newTint);

		/**
		 * Retrieve the program used to render this material
		 * @return the program used to render the material
		 */
		GLProgram * getProgram() const;

		virtual std::shared_ptr<Component> clone() const;

	private:
		GLProgram * programPtr;
		glm::vec3 _tint;
	};
}