#pragma once
#include "FactoryScenes.h"

enum class SidesDoor
{
	UP = 0,
	DOWN = 1,
	RIGHT = 2,
	LEFT = 3
};

struct LevelsScenes : public FactoryScenes
{
public:
	LevelsScenes(SDL_Renderer* pRenderer, const PATH& pPath, std::string_view pFirstScene);
	~LevelsScenes();

	void setBlackScreen() override;

	void loadNewScene(std::string_view pScene, const PATH& pPath) override;
	void removeScene(std::string_view pScene) override;

	void setScene(std::string_view pScene) override;
	void setScene(size_t pIterator);

	void render() override;

	size_t getSizeScenes() const noexcept;
	auto getPosDoors() -> std::unordered_map<SidesDoor, std::pair<Vector2f, Vector2f>>;
	auto getCondtitionOfDoors(std::string_view pScene) -> std::array<std::pair<SidesDoor, bool>, 4>;
	auto getNamesOfScenes() -> std::vector<std::string>;

private:
	
	void checkDoors(std::string_view pScene);
	//void setRangeOfPositions();

private:
	bool mBlackScene{ false };

	std::unordered_map<SidesDoor, std::pair<Vector2f, Vector2f>> mPositions =
	{
		{SidesDoor::UP,    std::make_pair(Vector2f(WIN_WIDTH / 2 - 150,0),                
										  Vector2f(WIN_WIDTH / 2 - 50, 5))},
		{SidesDoor::DOWN,  std::make_pair(Vector2f(WIN_WIDTH / 2 - 150, WIN_HEIGHT - 110),
										  Vector2f(WIN_WIDTH / 2 - 50, WIN_HEIGHT - 110))},
		{SidesDoor::LEFT,  std::make_pair(Vector2f(0, WIN_HEIGHT / 2 - 140),              
								          Vector2f(5, WIN_HEIGHT / 2 - 10))},
		{SidesDoor::RIGHT, std::make_pair(Vector2f(WIN_WIDTH - 90, WIN_HEIGHT / 2 - 150), 
			                              Vector2f(WIN_WIDTH - 95, WIN_HEIGHT / 2 - 50))}
	};
	std::unordered_map<std::string, std::array<std::pair<SidesDoor, bool>, 4>> mStorageDoors;
	std::vector<std::string> mNamesOfScenes;

	SDL_Rect mScreenRect{ 1,1,1,1 };
	SDL_Renderer* mRenderer{ nullptr };

	std::string mCurrentScene;
	PATH mCurrentPath{ std::filesystem::current_path() };

};

