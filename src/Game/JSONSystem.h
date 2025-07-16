#pragma once
#include <filesystem>
#include <fstream>

#include "Loger/Loging.h"
#include "json/json.hpp"

using PATH = std::filesystem::path;

class JSONSystem
{
public:
	JSONSystem(const PATH& pPath);
	~JSONSystem();
	
	void removeFile(const PATH& pPath);
	void loadFile(const PATH& pPath);

	std::string readFromFile();
	std::string readFromFile(std::string_view pName);
	void writeIntoFile(std::string_view pName, std::string_view pData);
	
	//if we have multiple files
	std::string readFromFile(const PATH& pPath);
	void writeIntoFile(const PATH& pPath, std::string_view pName, std::string_view pData);

private:
	bool ensureFileOpened(const std::fstream& pStream);

private:
	std::fstream mStream;
	nlohmann::json mJsonData;

	PATH mPath;
};

