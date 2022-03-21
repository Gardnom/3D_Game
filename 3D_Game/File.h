#pragma once
#include <string>
#include <fstream>
#include <sstream>

class File {
private:
	std::ifstream m_File;
public:
	File(std::string& path) {
		m_File = std::ifstream(path);
		if (!m_File.is_open()) {
			printf("Failed to open file!\n");
		}

	}

	~File() {
		close();
	}

	bool Getline(std::string& line) {
		if (!m_File.is_open()) {
			printf("You must open file before getting line");
			return false;
		}
		std::getline(m_File, line);
		return true;
	}

	void close() {
		m_File.close();
	}
	
	static std::ifstream OpenFile(std::string& path) {
		return std::ifstream(path);
	}

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

	static std::string BrowseForFile(std::string startPath);
};