#include "LevelsScenes.h"

LevelsScenes::LevelsScenes(SDL_Renderer* pRenderer, const PATH& pPath, std::string_view pFirstScene)
	: mRenderer( pRenderer ),
	  mCurrentPath{ pPath }
{
	if (std::filesystem::exists(pPath))
	{
		mScenes.insert_or_assign(std::string(pFirstScene), pPath);
		TextureManager::getInstance().appendTexture(mRenderer, pPath, mScreenRect);
		mCurrentScene = pFirstScene;
		checkDoors(pFirstScene);
		mNamesOfScenes.push_back(std::string(pFirstScene));
	}
	else
		LOG("this path doesnt exist!");
}

LevelsScenes::~LevelsScenes()
{
	SDL_DestroyRenderer(mRenderer);
}

void LevelsScenes::setBlackScreen()
{
	mBlackScene = true;
}

void LevelsScenes::loadNewScene(std::string_view pScene, const PATH& pPath)
{
	if (std::filesystem::exists(pPath))
	{
		mScenes.insert_or_assign(std::string(pScene), pPath);
		TextureManager::getInstance().appendTexture(mRenderer, pPath, mScreenRect);
		mCurrentScene = pScene;
		checkDoors(pScene);
		mNamesOfScenes.push_back(std::string(pScene));
	}
	else
		LOG("this path doesnt exist!");
}

void LevelsScenes::removeScene(std::string_view pScene)
{
	if (mScenes.contains(std::string(pScene)))
	{
		auto it = mScenes.find(std::string(pScene));
		TextureManager::getInstance().deleteTexture(it->second);
		mCurrentScene = (it--)->first;
		mScenes.erase(it);
		std::remove(mNamesOfScenes.begin(), mNamesOfScenes.end(), std::string(pScene));
	}
	else
		LOG("this name doesnt exist!");
}

void LevelsScenes::setScene(std::string_view pScene)
{
	if (mScenes.contains(std::string(pScene)))
		mCurrentScene = pScene;
	else
		LOG("this name doesnt exist!");
}

void LevelsScenes::setScene(size_t pIterator)
{
	if (pIterator >= mScenes.size())
		LOG("your number is bigger than size!");
	else
	{
		auto it = mScenes.begin();
		std::advance(it, pIterator);
		mCurrentScene = it->first;
	}
}

void LevelsScenes::render()
{
	if (mBlackScene)
	{
		SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 0);
		SDL_RenderClear(mRenderer);
	}
	else
	{
		SDL_RenderCopy(mRenderer, TextureManager::getInstance().getTexture(getScene(mCurrentScene)),
					   nullptr, &mScreenRect);
	}
}

size_t LevelsScenes::getSizeScenes() const noexcept
{
	return mScenes.size();
}

auto LevelsScenes::getPosDoors() -> std::unordered_map<SidesDoor, std::pair<Vector2f, Vector2f>>
{
	return mPositions;
}

auto LevelsScenes::getCondtitionOfDoors(std::string_view pScene) -> std::array<std::pair<SidesDoor, bool>, 4>
{
	if (mStorageDoors.contains(std::string(pScene)))
		return mStorageDoors[std::string(pScene)];
	LOG("There is no such a name!");
	return
	{
		std::make_pair(SidesDoor::UP,    false),
		std::make_pair(SidesDoor::DOWN,  false),
		std::make_pair(SidesDoor::LEFT,  false),
		std::make_pair(SidesDoor::RIGHT, false)
	};
}

auto LevelsScenes::getNamesOfScenes() -> std::vector<std::string>
{
	return mNamesOfScenes;
}

void LevelsScenes::checkDoors(std::string_view pScene)
{
	int it = 0;
	std::array<std::pair<SidesDoor, bool>, 4> mProperVarDoors;
	for (auto& i : mPositions)
	{
		for (int y = i.second.first.mY; y < i.second.first.mY; ++y)
		{
			for (int x = i.second.first.mX; x < i.second.second.mX; ++x)
			{
				SDL_Color tmpColor = TextureManager::getInstance().getBackGroundColor(getScene(pScene), Vector2f(x, y));
				if (tmpColor.r == 136 && tmpColor.b == 21 && tmpColor.g == 0)
				{
					if (it == 0)
						mProperVarDoors[it] = { SidesDoor::UP,    false };
					else if (it == 1)
						mProperVarDoors[it] = { SidesDoor::DOWN,  false };
					else if (it == 2)
						mProperVarDoors[it] = { SidesDoor::LEFT,  false };
					else if (it == 3)
						mProperVarDoors[it] = { SidesDoor::RIGHT, false };
					it++;
					break;
				}
				if (tmpColor.r == 34 && tmpColor.b == 76 && tmpColor.g == 177)
				{
					if (it == 0)
						mProperVarDoors[it] = { SidesDoor::UP,    true};
					else if (it == 1)
						mProperVarDoors[it] = { SidesDoor::DOWN,  true };
					else if (it == 2)
						mProperVarDoors[it] = { SidesDoor::LEFT,  true };
					else if (it == 3)
						mProperVarDoors[it] = { SidesDoor::RIGHT, true };
					it++;
					break;
				}
			}
		}
	}
	mStorageDoors.insert_or_assign(std::string(pScene), mProperVarDoors);
}

//void LevelsScenes::setRangeOfPositions()
//{
//	std::array<SidesDoor, 4> tmpSide =
//	{
//		SidesDoor::UP,
//		SidesDoor::DOWN,
//		SidesDoor::LEFT,
//		SidesDoor::RIGHT
//	};
//	for (int i = 0; i < 4; ++i)
//	{
//		for (int y = mPositions[tmpSide[i]].first.mY; y < mPositions[tmpSide[i]].second.mY; ++y)
//		{
//			for (int x = mPositions[tmpSide[i]].first.mX; x < mPositions[tmpSide[i]].second.mX; ++x)
//			{
//				mRangeOfPositions[tmpSide[i]].push_back(Vector2f(x, y));
//			}
//		}
//	}

//}
