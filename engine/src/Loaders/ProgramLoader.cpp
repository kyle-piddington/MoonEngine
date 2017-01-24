#include "ProgramLoader.h"
#include "thirdparty/rapidjson/document.h"
#include "thirdparty/rapidjson/error/error.h"
#include "thirdparty/rapidjson/error/en.h"
#include "IO/TextLoader.h"
#include "Util/Logger.h"
#include "GLUtil/GLProgramUtilities.h"
using namespace MoonEngine;

bool verifyFile(const rapidjson::Document & document)
{
	bool shaderOK = true;
	shaderOK &= (document.HasMember("name"));
	shaderOK &= (document["name"].IsString());
	if(!shaderOK)
	{
		LOG(ERROR, "Shader Could not be loaded, no name");
		return shaderOK;
	}
	shaderOK &= (document.HasMember("vertex"));
	shaderOK &= (document["vertex"].IsString());
	if(!shaderOK)
	{
		LOG(ERROR, "Shader from program " + std::string(document["name"].GetString()) + " Could not be loaded, no vertex shader tag");
		return shaderOK;
	}
	shaderOK &= (document.HasMember("fragment"));
	shaderOK &= (document["fragment"].IsString());
	if(!shaderOK)
	{
		LOG(ERROR, "Shader from program " + std::string(document["name"].GetString()) + " Could not be loaded, no fragment shader tag");
		return shaderOK;
	}

	return shaderOK;
}

std::shared_ptr<GLProgram> ProgramLoader::LoadProgramFromFile(std::string programName)
{
	std::string fileContents = TextLoader::LoadFullFile(programName);
	return ProgramLoader::LoadProgram(fileContents);

}
std::shared_ptr<GLProgram> ProgramLoader::LoadProgram(std::string programInfo)
{
	//Try and open the file first
	//Parse JSON
	rapidjson::Document document;
	rapidjson::ParseResult pr = document.Parse(programInfo.c_str());
	if(!pr)
	{
		LOG(ERROR, "ERROR: Could not parse document json. ( " + 
			std::string(rapidjson::GetParseError_En(pr.Code())) + " ["  + std::to_string(pr.Offset()) + "] )");
		return nullptr;
	}
	verifyFile(document);
	std::string shaderName = document["name"].GetString();
	std::string vertexShaderSource = document["vertex"].GetString();
	std::string fragmentShaderSource = document["fragment"].GetString();

	std::shared_ptr<GLProgram> glProgram = std::make_shared<GLProgram>(shaderName);
	//Create shaders
	GLShader vShader = 
		GLProgramUtilities::createShaderFromFile(GL_VERTEX_SHADER, vertexShaderSource);
	if(!GLProgramUtilities::checkShaderStatus(vShader))
	{
		return nullptr;
	}
	GLShader fShader = 
		GLProgramUtilities::createShaderFromFile(GL_FRAGMENT_SHADER, fragmentShaderSource);
	if(!GLProgramUtilities::checkShaderStatus(fShader))
	{
		return nullptr;
	}

	glProgram->attachShader(vShader);
	glProgram->attachShader(fShader);
	if(document.HasMember("geometry") && document["geometry"].IsString())
	{
		GLShader gShader = 
			GLProgramUtilities::createShaderFromFile(GL_GEOMETRY_SHADER, document["geometry"].GetString());
		if(!GLProgramUtilities::checkShaderStatus(gShader))
		{
			return nullptr;
		}
		glProgram->attachShader(gShader);
	}
	glProgram->link();
	if(!GLProgramUtilities::checkProgramStatus(*glProgram))
	{
		return nullptr;
	}
	return glProgram;


	
	

}