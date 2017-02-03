#include "TextLoader.h"
#include "Util/Logger.h"
#include <fstream>

using namespace MoonEngine;

std::string TextLoader::LoadFullFile(std::string fileName)
{
    try
    {
        std::ifstream inputStr(fileName);
        std::string str = "";
        if (inputStr)
        {

            inputStr.seekg(0, std::ios::end);
            str.reserve(inputStr.tellg());
            inputStr.seekg(0, std::ios::beg);

            str.assign((std::istreambuf_iterator<char>(inputStr)),
                       std::istreambuf_iterator<char>());
            inputStr.close();
        }
        else
        {
            LOG(ERROR, "Could not load file " + fileName);
        }
        return str;
    }
    catch (std::ifstream::failure e)
    {
        LOG(ERROR, "Error opening file");
        return "";
    }

}