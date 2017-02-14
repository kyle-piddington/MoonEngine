#include <IO/TextLoader.h>
#include <Util/Logger.h>
#include <thirdparty/rapidjson/document.h>
#include <thirdparty/rapidjson/error/en.h>
#include <iostream>
#include "MoonEngineCfg.h"

using namespace MoonEngine;

MoonEngineCfg::MoonEngineCfg(std::string configName)
{
    LoadConfigFromFile(configName);
}

bool verifyConfigFile(const rapidjson::Document & document)
{
    bool configOK = true;
    configOK &= (document.HasMember("assetPath"));
    configOK &= (document["assetPath"].IsString());
    if (!configOK)
    {
        LOG(ERROR, "Config could not be loaded, no assetPath");
        return configOK;
    }
    configOK &= (document.HasMember("assetDirectories"));
    if (!configOK)
    {
        LOG(ERROR, "Config could not be loaded, no directories");
        return configOK;
    }

    return configOK;
}

void MoonEngineCfg::LoadConfigFromFile(std::string configName)
{
    std::string fileContents = TextLoader::LoadFullFile(configName);
    return MoonEngineCfg::LoadConfig(fileContents);

}

void MoonEngineCfg::LoadConfig(std::string configInfo)
{
    //Try and open the file first
    //Parse JSON
    rapidjson::Document document;
    rapidjson::ParseResult pr = document.Parse(configInfo.c_str());
    if (!pr)
    {
        LOG(ERROR, "ERROR: Could not parse document json. ( " + std::string(rapidjson::GetParseError_En(pr.Code())) +
                   " [" + std::to_string(pr.Offset()) + "] )");
        return;
    }
    verifyConfigFile(document);
    assetPath = document["assetPath"].GetString();

    for (rapidjson::Value::ConstMemberIterator dirs = document["assetDirectories"].MemberBegin(); dirs != document["assetDirectories"].MemberEnd(); ++dirs)
    {
        asset_directories[dirs->name.GetString()] = dirs->value.GetString();
    }
}