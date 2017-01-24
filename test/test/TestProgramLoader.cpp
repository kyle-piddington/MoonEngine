#include "catch.hpp"
#include "MoonEngine.h"
using namespace MoonEngine;

TEST_CASE("Load program from json", "[Loaders]")
{
	std::shared_ptr<GLProgram> prog  = ProgramLoader::LoadProgramFromFile("resources/flat.program");
	REQUIRE(prog != nullptr);
}

TEST_CASE("Load program from json err", "[Loaders]")
{
	std::shared_ptr<GLProgram> prog  = ProgramLoader::LoadProgramFromFile("resources/flat-bad.program");
	REQUIRE(prog == nullptr);
}