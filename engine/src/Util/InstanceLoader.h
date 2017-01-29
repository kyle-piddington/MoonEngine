#pragma once
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include "glm\glm.hpp"
using namespace std;
namespace MoonEngine
{
	namespace Util {
		
		inline vector<glm::mat4> InstanceLoader(string fileName) {
			
			vector<glm::mat4> result;
			glm::vec3 pos;
			glm::vec3 scl;
			glm::vec3 rot;
			std::ifstream File(fileName);
			std::string line;
			while (std::getline(File, line))
			{
				std::istringstream iss(line);
				if (!(iss >> pos.x >> pos.y >> pos.z)) { 
					break; // error in position
				} 
				if (!(iss >> scl.x >> scl.y >> scl.z)) {
					break; // error in scale
				}
				if (!(iss >> rot.x >> rot.y >> rot.z)) {
					break; // error in rotation
				}						
			}
			return  
		}

	}
}