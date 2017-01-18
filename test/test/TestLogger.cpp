#include "catch.hpp"
#include "MoonEngine.h"
#include <sstream>

using namespace MoonEngine;

TEST_CASE("Logger redirects osteram" , "[Logger]")
{
	std::stringstream writableStream;
	Logger::ProvideErrorStream(&writableStream);
	LOG(FATAL_ERROR, "testCase");
	REQUIRE("[!FATAL ERROR!]: testCase\n" == writableStream.str());
}
TEST_CASE("Logger ignores loglevel" , "[Logger]")
{
	std::stringstream writableStream;
	Logger::ProvideErrorStream(&writableStream);
	LOG(VERBOSE, "testCase");
	REQUIRE(writableStream.str().empty());
}
TEST_CASE("Logger sets loglevel" , "[Logger]")
{
	std::stringstream writableStream;
	Logger::ProvideErrorStream(&writableStream);
	Logger::SetLogLevel(VERBOSE);
	LOG(VERBOSE, "testCase");
	REQUIRE(writableStream.str()== "[Log]: testCase\n");
}
