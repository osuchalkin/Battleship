#pragma once

#include <SFML/Graphics.hpp> 
#include <memory>
#include <array>
#include <random> 

#include "Constants.h"
#include "Grid.h"
#include "Fleet.h"

class World
{
public:
	World();	
	void update(sf::Time deltaTime);
	void render(sf::RenderWindow& window);
	void checkEvents(sf::RenderWindow& window, const sf::Event& event);

private:
	int getRandNum(int first, int last);
	void generateWorld();
	void setupText();
	void setupPlayerFleet(sf::Vector2i mousePos);
	void setupCells();
	void renderCells(sf::RenderWindow& window);
	void setupComputerFleet();
	void checkPlayerShot(sf::Vector2i mousePos);
private:
	enum class GameState
	{
		Preparing,
		Playing,
		GameOver
	};
	std::mt19937 mt;

	GameState mGameState;
	sf::Texture mOceanGridTexture;
	sf::Sprite mOceanGridSprite;
	sf::Texture mRadarGridTexture;
	sf::Sprite mRadarGridSprite;

	sf::Font mFont;
	sf::Text mTitle;
	sf::Text mInfoText;

	std::unique_ptr<Fleet> mFleetPrepare;
	std::unique_ptr<Grid> mPlayerGrid;
	std::array<std::array<sf::RectangleShape, CELL_WIDTH>, CELL_HEIGHT> mCells;
	std::unique_ptr<Grid> mComputerGrid;
	std::array<std::array<sf::RectangleShape, CELL_WIDTH>, CELL_HEIGHT> mCompCells;
	sf::CircleShape mShotIndicator;

	std::unique_ptr<Fleet> mPlayerFleet;
	std::unique_ptr<Fleet> mComputerFleet;

	std::string mTurn; // "player" or "computer"
};

