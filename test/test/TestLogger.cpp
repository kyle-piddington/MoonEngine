#include "catch.hpp"
#include "MoonEngine.h"
#include <sstream>
#include <iostream>
using namespace MoonEngine;

TEST_CASE("Logger redirects osteram" , "[Logger]")
{
	std::stringstream writableStream;
	Logger::ProvideErrorStream(&writableStream);
	LOG(FATAL_ERROR, "testCase");
	REQUIRE("[!FATAL ERROR!]: testCase\n" == writableStream.str());
	Logger::ProvideErrorStream(nullptr);
}
TEST_CASE("Logger ignores loglevel" , "[Logger]")
{
	std::stringstream writableStream;
	Logger::ProvideErrorStream(&writableStream);
	LOG(VERBOSE, "testCase");
	REQUIRE(writableStream.str().empty());
	Logger::ProvideErrorStream(nullptr);
}
TEST_CASE("Logger sets loglevel" , "[Logger]")
{
	std::stringstream writableStream;
	Logger::ProvideErrorStream(&writableStream);
	Logger::SetLogLevel(VERBOSE);
	LOG(VERBOSE, "testCase");
	REQUIRE(writableStream.str()== "[Log]: testCase\n");
	Logger::ProvideErrorStream(nullptr);
}
