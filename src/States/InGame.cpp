#include "InGame.h"

InGame::InGame(SDL_Renderer* pRenderer, const PATH& pScenePath, const PATH& pPathMusic,
			   Character* pCharacter)
		: mLvlScenes( pRenderer,pScenePath,"FirstScene" )
{
	mCurrentScene = "FirstScene";
	mCharacter = pCharacter;
	mAudio.setMusic(Music::GAME, pPathMusic);
	mAudio.setVolumeMusic(Music::GAME, 5);
	mLvlScenes.loadNewScene("SecondScene", std::filesystem::current_path() / "Assets" / "photos and ttf" / "second.png");
	mBuffer.reserve(20);
	mRandomizer.setEnd(mLvlScenes.getNamesOfScenes().size());
}

InGame::~InGame()
{
	delete mCharacter;
}

void InGame::hopIn()
{
	if (!mAudio.isPlaying())
		mAudio.playMusic(Music::GAME);
	mActive = true;
}

void InGame::getOutta()
{
	mAudio.stopMusic(Music::GAME);
	mActive = false;
}

void InGame::handleEvents(std::weak_ptr<Game> pGame)
{
#ifdef _DEBUG
		for (auto& i : mLvlScenes.getPosDoors())
		{
			std::cout << std::format("Position of a door: {} - {}, {} - {}\n", i.second.first.mX, i.second.first.mY, i.second.second.mX, i.second.second.mY);
		}
#endif // _DEBUG
		
		SidesDoor tmpSide = SidesDoor::UP;
		for (uint16_t side = 0; side < 4; ++side)
		{
			if ((isInTheRange(mCharacter->getPos(), tmpSide) && mLvlScenes.getCondtitionOfDoors(mCurrentScene)[side].second))
			{
				mRandomizer.generateNew();
				for (size_t tmp = 0; tmp < mBuffer.size(); ++tmp)
				{
					if (mBuffer[tmp] == mRandomizer.getNumber())
					{
						mRandomizer.generateNew();
						tmp = 0;
					}
				}
				mCurrentScene = mLvlScenes.getNamesOfScenes()[mRandomizer.getNumber()];
				tmpSide = SidesDoor::UP;
				mBuffer.push_back(mRandomizer.getNumber());
				if (tmpSide == SidesDoor::UP)
					mCharacter->setPos(Vector2f(WIN_WIDTH / 2, WIN_HEIGHT - 100));
				else if(tmpSide == SidesDoor::DOWN)
					mCharacter->setPos(Vector2f(WIN_WIDTH / 2, 5));
				else if(tmpSide == SidesDoor::RIGHT)
					mCharacter->setPos(Vector2f(0, WIN_HEIGHT / 2));
				else if (tmpSide == SidesDoor::LEFT)
					mCharacter->setPos(Vector2f(WIN_WIDTH - 90, WIN_HEIGHT / 2));
				break;
			}
			tmpSide = static_cast<SidesDoor>(side);
		}
}

void InGame::render()
{
	update();
	mLvlScenes.render();
}

void InGame::update()
{
	mLvlScenes.setScene(mCurrentScene);
	mRandomizer.setEnd(mLvlScenes.getNamesOfScenes().size());
}

bool InGame::isActive()
{
	return mActive;
}

bool InGame::isInTheRange(const Vector2f& pPosEntity, const SidesDoor pSides)
{
	static auto mPosDoors = mLvlScenes.getPosDoors();
	if (pPosEntity.mX > mPosDoors[pSides].first.mX  &&
		pPosEntity.mX < mPosDoors[pSides].second.mX &&
		pPosEntity.mY > mPosDoors[pSides].first.mY  &&
		pPosEntity.mY < mPosDoors[pSides].second.mY)
		return true;
	return false;
}
