#include "Loging.h"

void Loging::log(std::string_view pMessage, const std::filesystem::path& pFile, const int& pLine)
{
	std::lock_guard<std::mutex> guard(mutex);

	auto time = std::chrono::system_clock::now();
	auto nowTime = std::chrono::system_clock::to_time_t(time);

	std::tm vTm;
	localtime_s(&vTm, &nowTime);

	std::ostringstream timeStamp;
	std::cout << std::put_time(&vTm, "%Y-%m-%d %H:%M:%S");

	std::cerr << "[ERROR][" << timeStamp.str() << ']' << "[SDL_ERROR: " << SDL_GetError() << ']';
	if (!pFile.empty())
	{
		std::cerr << '[' << pFile.string() << ']';
		if (pLine > 0)
			std::cerr << "[LINE: " << pLine << ']';
	}
	std::cout << "[MESSAGE]" << '[' << message << ']' << '\n';
}

void Loging::message(std::string_view pMessage)
{
	std::lock_guard<std::mutex> lg(mutex);

	auto time = std::chrono::system_clock::now();
	auto nowTime = std::chrono::system_clock::to_time_t(time);

	std::tm vTm;
	localtime_s(&vTm, &nowTime);

	std::cout << "[TIME: " << std::put_time(&vTm, "%Y-%m-%d %H:%M:%S") << ']' << "[MESSAGE]" << '[' << pMessage << "]\n";
}
