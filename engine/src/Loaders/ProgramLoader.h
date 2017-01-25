/**
 * Load an OBJ model and read it into an interchange mesh format.
 */

#include "GL/GLProgram.h"
#include <string>
#include <memory>
namespace MoonEngine
{
	class ProgramLoader
	{
	public:
		static std::shared_ptr<GLProgram> LoadProgramFromFile(std::string fileName);
		static std::shared_ptr<GLProgram> LoadProgram(std::string programInfo);


	};
};