#pragma once

#include "Component/Component.h"
#include <glm/glm.hpp>

#include "GLWrapper/GLProgram.h"
#include <GLWrapper/GLSampler.h>
#include <GLWrapper/GLTexture.h>
#include <vector>
/**
 * Material contains the basic rendering
 * information needed for rendering a material. In this case,
 * a program to determine how this material should be rendered,
 * and a few backup material properties, such as tint.
 */

namespace MoonEngine
{
    class Material: public Component
    {
    public:
        //TODO add sampler
        Material(glm::vec3 tint = glm::vec3(0, 0, 0),
            std::string programName = "default.program",
            unordered_map<string, string> textures = unordered_map<string, string>(),
			bool forward = false
		);

        Material(std::string programName, bool forward = false);


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
        virtual void setTint(glm::vec3 newTint);

        /**
         * Retrieve the program used to render this material
         * @return the program used to render the material
         */
        GLProgram * getProgram() const;




        virtual std::shared_ptr<Component> clone() const;

        virtual void bind();


		virtual bool hasShadowProgram() {
			return false;
		}
		virtual GLProgram * getShadowProgram() const
		{
			return nullptr;
		}

		virtual void bindForShadow() {};

		inline bool isForward() {
			return _forward;
		};

        void unbind();

        void addTexture(std::string uniformName, GLTexture * texture);
        
        void loadTextures(std::unordered_map<string, string> textures);

    protected:
         struct texture_unit {
            GLTexture * gl_texture;
            GLuint unit;
        };
        GLSampler * _sampler;
        GLProgram * _program;
        GLuint _texture_unit;
        bool _forward;
        glm::vec3 _tint;    
        unordered_map<string, texture_unit> _textures;
     
		
    };
}

typedef std::unordered_map<std::string, std::string> stringmap;