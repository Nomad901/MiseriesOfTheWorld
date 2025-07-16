#include "Game/Assets.h"
#include "Game/TextureManager.h"
#include "Game objets/FactoryOfFonts.h"

auto Assets::getAnimation(const PATH& pPath) -> std::unordered_map<int, std::pair<SDL_Texture*, SDL_Rect>>
{
    if (mAnimation.find(pPath) == mAnimation.end())
    {
        LOG("couldnt find the path!");
        return { {0,{nullptr,{0,0,0,0}}} };
    }
    mAssets[0].writeIntoFile(pPath.string(), "anim");
    return mAnimation[pPath];
}

auto Assets::getTexture(const PATH& pPath) -> SDL_Texture*
{
    if (mTextures.find(pPath) == mTextures.end())
    {
        TextureManager::getInstance().appendTexture(mRenderer, pPath, { 100,100,100,100 });
        mAssets[1].writeIntoFile(pPath.string(), "textures");
    }

    return mTextures[pPath].first.second;
}
 
auto Assets::getSurface(const PATH& pPath) -> SDL_Surface*
{
    if (mTextures.find(pPath) == mTextures.end())
    {
        TextureManager::init();
        TextureManager::getInstance().appendTexture(mRenderer, pPath, { 100,100,100,100 });
        mAssets[2].writeIntoFile(pPath.string(), "surface");
        TextureManager::shutDown();
    }

    return mTextures[pPath].first.first;
}

auto Assets::getFonts(std::string_view pAlias) -> TTF_Font*
{
    if (mFonts.find(std::string(pAlias)) != mFonts.end())
    {
        mAssets[3].writeIntoFile(std::string(pAlias), "font");
        return mFonts[std::string(pAlias)].first;
        //if (!mFonts[std::string(pAlias)].first)
        //    LOG("Couldnt itialize the path or the font: " + std::string(TTF_GetError()));
    }
    return nullptr;
}

auto Assets::getTextureFont(std::string_view pAlias) -> SDL_Texture*
{
    if (mFonts.find(std::string(pAlias)) != mFonts.end())
    {
        mAssets[3].writeIntoFile(std::string(pAlias), "font");
        return mFonts[std::string(pAlias)].second;
    }
    return nullptr;
}

auto Assets::getSound(const PATH& pPath) -> Mix_Chunk*
{
    if (mSounds.find(pPath) == mSounds.end())
    {
        mSounds.insert_or_assign(pPath, Mix_LoadWAV(pPath.string().c_str()));
        if(!mSounds[pPath])
            LOG("Couldnt itialize the path or the sound: " + std::string(Mix_GetError()));
        mAssets[4].writeIntoFile(pPath.string(), "sounds");
    }
    return mSounds[pPath];
}

auto Assets::getMusic(const PATH& pPath) -> Mix_Music*
{
    if (mMusic.find(pPath) == mMusic.end())
    {
        mMusic.insert_or_assign(pPath,Mix_LoadMUS(pPath.string().c_str()));
        if(!mMusic[pPath])
            LOG("Couldnt itialize the path or the music: " + std::string(Mix_GetError()));
        mAssets[5].writeIntoFile(pPath.string(), "music");
    }
    return mMusic[pPath];
}

auto Assets::getScene(std::string_view pScene) -> PATH
{
    if (!mScenes.contains(std::string(pScene)))
    {
        LOG("there is no such a scene!");
        return "";
    }
    return mScenes[std::string(pScene)];
}

Assets::~Assets()
{
    for (auto& i : mMusic)
    {
        Mix_FreeMusic(i.second);
    }
    mMusic.clear();
    for (auto& i : mSounds)
    {
        Mix_FreeChunk(i.second);
    }
    mSounds.clear();
    for (auto& i : mTextures)
    {
        SDL_FreeSurface(i.second.first.first);
        SDL_DestroyTexture(i.second.first.second);
    }
    mTextures.clear();
    for (auto& i : mFonts)
    {
        TTF_CloseFont(i.second.first);
        SDL_DestroyTexture(i.second.second);
    }
    mFonts.clear();

    SDL_DestroyRenderer(mRenderer);
}

void Assets::setRenderer(SDL_Renderer* pRenderer)
{
    mRenderer = pRenderer;
}
