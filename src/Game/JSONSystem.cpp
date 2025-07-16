#include "JSONSystem.h"

JSONSystem::JSONSystem(const PATH& pPath)
{
	mStream.open(pPath);
	if (ensureFileOpened(mStream))
	{
		if (mStream.tellg() == 0)
			mJsonData = nlohmann::json::object();
		else
			mJsonData = nlohmann::json::parse(mStream);
	}
	mPath = pPath;
}

JSONSystem::~JSONSystem()
{
	if (mStream.is_open())
		mStream.close();
}

void JSONSystem::removeFile(const PATH& pPath)
{
	if (ensureFileOpened(mStream))
		mStream.close();
	if (std::filesystem::exists(pPath))
		std::filesystem::remove(pPath);
}

void JSONSystem::loadFile(const PATH& pPath)
{
	if (ensureFileOpened(mStream))
		mStream.close();
	mStream.open(pPath);
	if (ensureFileOpened(mStream))
		mJsonData = nlohmann::json::parse(mStream);
	mPath = pPath;
}

std::string JSONSystem::readFromFile()
{
	if (ensureFileOpened(mStream))
		return mJsonData.dump();
	return "";
}

std::string JSONSystem::readFromFile(std::string_view pName)
{
	if (ensureFileOpened(mStream))
	{
		if(mJsonData.contains(pName))
			return mJsonData[std::string(pName)].dump();
	}
	return "";
}

void JSONSystem::writeIntoFile(std::string_view pName, std::string_view pData)
{
	if (ensureFileOpened(mStream))
	{
		mJsonData[std::string(pName)] = pData;

		mStream.close();
		mStream.open(mPath, std::ios::out | std::ios::trunc);
		mStream << std::setw(4) << mJsonData;
	}
}

std::string JSONSystem::readFromFile(const PATH& pPath)
{
	std::fstream fst(pPath);
	if(ensureFileOpened(fst))
	{
		auto tmpJson = nlohmann::json::parse(fst);
		return tmpJson.dump();
	}
	return "";
}

void JSONSystem::writeIntoFile(const PATH& pPath, std::string_view pName, std::string_view pData)
{
	nlohmann::json tmpData;

	if (std::filesystem::exists(pPath))
	{
		std::fstream ifstr(pPath);
		if (ensureFileOpened(ifstr))
			tmpData = nlohmann::json::parse(ifstr);
	}

	tmpData[std::string(pName)] = pData;

	std::fstream ofstr(pPath, std::ios::trunc);
	if (ensureFileOpened(ofstr))
		ofstr << std::setw(4) << tmpData;
}

bool JSONSystem::ensureFileOpened(const std::fstream& pStream)
{
	if (!pStream.is_open())
	{
		LOG("couldnt find the file");
		return false;
	}
	return true;
}
