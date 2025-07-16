#include "FireModeFactory.h"

auto FireModeFactory::appendMode(std::string_view pName, std::unique_ptr<FireMode> pMode) -> void
{
	if (!mStorageFireModes.contains(std::string(pName)))
	{
		mStorageFireModes.insert_or_assign(std::string(pName), std::move(pMode));
		mStorageLabels.emplace(std::string(pName));
		mCurrentFireMode = std::string(pName);
	}
	else
		LOG("The storage already have this!");
}

auto FireModeFactory::replaceMode(std::string_view pName, std::unique_ptr<FireMode> pMode) -> void
{
	if (mStorageFireModes.contains(std::string(pName)))
	{
		mStorageFireModes.insert_or_assign(std::string(pName), std::move(pMode));
		mStorageLabels.emplace(std::string(pName));
		mCurrentFireMode = std::string(pName);
	}
	else
		LOG("The storage doesnt have this!");
}

auto FireModeFactory::setMode(std::string_view pName) -> void
{
	if (mStorageFireModes.contains(std::string(pName)))
	{
		mStorageLabels.emplace(std::string(pName));
		mCurrentFireMode = std::string(pName);
	}
	else
		LOG("The storage doesnt have this!");
}

auto FireModeFactory::isExist(std::string_view pName) -> bool
{
	if(mStorageFireModes.contains(std::string(pName)))
		return true;
	return false;
}

auto FireModeFactory::getNameCurrentMode() -> std::string
{
	return mCurrentFireMode;
}

auto FireModeFactory::getMode() -> std::expected<std::reference_wrapper<FireMode>, std::string_view>
{
	auto it = mStorageFireModes.find(mCurrentFireMode);
	if (it != mStorageFireModes.end()) {
		mCurrentFireMode = it->first;
		return std::ref(*it->second);
	}
	LOG("U didnt set the mode!");
	return std::unexpected("U didnt set the mode!");
}

auto FireModeFactory::getMode(std::string_view pName) -> std::expected<std::reference_wrapper<FireMode>, std::string_view>
{
	auto it = mStorageFireModes.find(std::string(pName));
	if (it != mStorageFireModes.end()) {
		mCurrentFireMode = it->first; 
		return std::ref(*it->second); 
	}
	LOG("This mode doesn't exist!");
	return std::unexpected("This mode doesn't exist!");
}

auto FireModeFactory::getStorageLabels() -> std::unordered_set<std::string>
{
	return mStorageLabels;
}

