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

        Material(glm::vec3 tint = glm::vec3(0, 0, 0),
            std::vector<string> programNames = {"default.program"},
            unordered_map<string, string> textures = unordered_map<string, string>(),
            bool forward = false
        );

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

        /* Set the currently active program for material */
        void setActiveProgram(int program);

        virtual std::shared_ptr<Component> clone() const;

        void bind();

		inline bool isForward() {
			return _forward;
		};

        void unbind();

    private:
        struct texture_unit {
            GLTexture * gl_texture;
            GLuint unit;
        };

        GLSampler * _sampler;

        int _activeProgram;
        vector<GLProgram *> _programs;

        glm::vec3 _tint;
        unordered_map<string, texture_unit> _textures;
        GLuint _texture_unit;

		bool _forward;
    };
}

typedef std::unordered_map<std::string, std::string> stringmap;