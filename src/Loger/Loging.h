#pragma once
#include <iostream>
#include <filesystem>
#include <mutex>
#include <chrono>
#include <sstream>

#include <SDL.h>

static std::mutex mutex;

namespace Loging {
	void log(std::string_view pMessage,
			 const std::filesystem::path& pFile,
			 const int& pLine);

	void message(std::string_view pMessage);
}

#define LOG(msg) Loging::log(msg, __FILE__,__LINE__)
#define MESSAGE(msg) Loging::message(msg)
