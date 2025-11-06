#pragma once

#include <SFML/Graphics.hpp> 
#include <memory>

#include "Fleet.h"
#include "TextInfo.h"
#include "Player.h"
#include "Computer.h"

class World
{
public:
	World();	
	void update(sf::Time deltaTime);
	void render(sf::RenderWindow& window);
	void checkEvents(sf::RenderWindow& window, const sf::Event& event);

private:
	void generateWorld();
	void startInit();
	void newGame();
private:
	enum class GameState
	{
		Preparing,
		Playing
	};

	GameState mGameState;
	sf::Texture mOceanGridTexture;
	sf::Sprite mOceanGridSprite;
	sf::Texture mRadarGridTexture;
	sf::Sprite mRadarGridSprite;

	std::unique_ptr<TextInfo> mTextInfo;

	std::unique_ptr<Fleet> mFleetPrepare;

	std::string mTurn; // "player" or "computer"
	std::string mWinner;

	std::unique_ptr<Player> mPlayer;
	std::unique_ptr<Computer> mComputer;

	bool mSoundOn;

	// Delay mechanism for computer turn
	sf::Time timeAccumulator = sf::Time::Zero; // Накопленное время задержки
	sf::Time moveDelay = sf::milliseconds(1500); // Требуемая задержка (1000 мс)
	bool isDelayActive = false; // Флаг, указывающий, ждем ли мы
};

