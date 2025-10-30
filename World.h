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
	void renderCells(sf::RenderWindow& window, std::string side, 
		Grid* grid, std::array<std::array<sf::RectangleShape, CELL_WIDTH>, CELL_HEIGHT>& cells);
	void setupComputerFleet();
	bool setComputerShip(TypeShip type, int setup);
	void checkPlayerShot(sf::Vector2i mousePos);
	void checkComputerShot();
	sf::Vector2i computerShootRandom();
	sf::Vector2i computerShoot();
	sf::Vector2i getCellForShot(TypeShip type);
	sf::Vector2i computerSeekShip();
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
	sf::VertexArray mCross;
	float lineLength = 50.0f;
	float offset = lineLength / 2.0f * (float)std::cos(45.0f * PI / 180.0f);

	std::unique_ptr<Fleet> mPlayerFleet;
	std::unique_ptr<Fleet> mComputerFleet;

	std::string mTurn; // "player" or "computer"
	std::string mWinner;

	bool mShipWounded;	// for computer shot
	std::vector<sf::Vector2i> mWoundedCells;
};

