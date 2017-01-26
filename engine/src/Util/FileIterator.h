#pragma once
#include <iostream>
#include <iterator>
#include <fstream>
#include <string>


/*	Example Usage: Prints the each line of the file back out
*	std::ifstream inputFile("input.txt");
*	istream_iterator<line> begin(inputFile);
*	istream_iterator<line> end;	
*	for (istream_iterator<line> it = begin; it != end; ++it)
*		cout << *it << "\n";
*/

using namespace std;
namespace MoonEngine
{
	namespace Util {
		class FileLine : public string {};

		inline istream &operator>>(std::istream &is, Util::FileLine &l) {
			std::getline(is, l);
			return is;
		}
	}
}