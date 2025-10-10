#pragma once

#include <SFML/Graphics.hpp> 
#include <memory>
#include <array>

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
	void generateWorld();
	void setupText();
	void setupPlayerFleet(sf::Vector2i mousePos);
	void setupCells();
	void renderCells(sf::RenderWindow& window);
private:
	enum class GameState
	{
		Preparing,
		Playing,
		GameOver
	};
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
};

