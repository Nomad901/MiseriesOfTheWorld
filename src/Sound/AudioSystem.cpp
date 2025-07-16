#include "Sound/AudioSystem.h"
#include "Game/Assets.h"

AudioSystem::AudioSystem()
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) < 0)
		LOG("Audio wasnt initilized!");
	if(!Mix_Init(MIX_INIT_FLAC|MIX_INIT_OGG|MIX_INIT_MP3))
		LOG("Audio wasnt initilized!");

}

AudioSystem::~AudioSystem()
{
	for (auto& i : mMusic)
	{
		Mix_FreeMusic(i.second);
	}
	mMusic.clear();
	for (auto& i : mChunk)
	{
		Mix_FreeChunk(i.second);
	}
	mChunk.clear();
	Mix_CloseAudio();
}

void AudioSystem::setSound(const Sounds& pType, const PATH& pPathSound)
{	
	mChunk.insert_or_assign(pType, Assets::getInstance().getSound(pPathSound));
}

void AudioSystem::setMusic(const Music& pType, const PATH& pPathMusic)
{
	mMusic.insert_or_assign(pType, Assets::getInstance().getMusic(pPathMusic));
}

void AudioSystem::setSound(const std::vector<Sounds>& pTypes, const std::vector<PATH>& pPathSounds)
{
	if (pTypes.size() > pPathSounds.size() || pTypes.size() < pPathSounds.size())
	{
		LOG("SMTH BIGGER THAN EXPECTED!");
		return;
	}
	for (size_t i = 0; i < pTypes.size()-1 ; i++)
	{
		mChunk.insert_or_assign(pTypes[i], Assets::getInstance().getSound(pPathSounds[i]));
	}
}

void AudioSystem::setMusic(const std::vector<Music>& pTypes, const std::vector<PATH>& pPathMusic)
{
	if (pTypes.size() > pPathMusic.size() || pTypes.size() < pPathMusic.size())
	{
		LOG("SMTH BIGGER THAN EXPECTED!");
		return;
	}
	for (size_t i = 0; i < pTypes.size() - 1; i++)
	{
		mMusic.insert_or_assign(pTypes[i], Assets::getInstance().getMusic(pPathMusic[i]));
	}
}

void AudioSystem::setVolumeMusic(const Music& pType, int pVolume)
{
	auto it = mMusic.find(pType);
	if (it == mMusic.end())
	{
		LOG("There is not such a type!");
		return;
	}
	else
		Mix_VolumeMusic(std::clamp(pVolume, 0, 128));
}

void AudioSystem::setVolumeSound(const Sounds& pType, int pVolume)
{
	auto it = mChunk.find(pType);
	if (it == mChunk.end())
	{
		LOG("There is not such a type!");
		return;
	}
	else
		Mix_VolumeChunk(mChunk[pType], std::clamp(pVolume, 0, 128));
}

void AudioSystem::setVolumeForAll(const int pVolume)
{
	Mix_VolumeMusic(std::clamp(pVolume, 0, 128));

	for (auto& i : mChunk)
	{
		Mix_VolumeChunk(i.second, std::clamp(pVolume, 0, 128));
	}
}

void AudioSystem::playSound(const Sounds& pType) const
{
	auto it = mChunk.find(pType);
	if (it == mChunk.end())
	{
		LOG("There is not such a type!");
		return;
	}
	else
		Mix_PlayChannel(-1, it->second, 0);
}

void AudioSystem::playMusic(const Music& pType)
{
	auto it = mMusic.find(pType);
	if (it == mMusic.end())
	{
		LOG("There is not such a type!");
		return;
	}
	else
	{
		mPlaying = true;
		Mix_PlayMusic(it->second, -1);
	}
}

void AudioSystem::stopMusic(const Music& pType)
{
	auto it = mMusic.find(pType);
	if (it == mMusic.end())
	{
		LOG("There is not such a type!");
		return;
	}
	else
	{
		mPlaying = false;
		Mix_PauseMusic();
	}
}
void AudioSystem::stopForAll(int pNumber) 
{
	Mix_PauseAudio(std::clamp(pNumber,0,1));
}
