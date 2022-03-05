#pragma once
#include <string>
#include <fstream>
#include <sstream>

class File {
public:
	static std::string ReadFile(std::string& path) {
		std::ifstream file(path);
		std::string fileContents;
		std::ostringstream ss;

		if (file.is_open()) {
			//fileContents = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
			ss << file.rdbuf();
			fileContents = ss.str();
		}
		else {
			printf("Could not open file!\n");
		}
		return fileContents;
	}
};