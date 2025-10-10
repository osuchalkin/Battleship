#pragma once

#include <SFML/Graphics.hpp> 
#include <memory>

#include "World.h"

class Game
{
public:
	Game();
	void run();
private:
	void processEvents();
	void update(sf::Time deltaTime);
	void render();

private:
	sf::RenderWindow mWindow;
	std::unique_ptr<World> mWorld;
	sf::Image mIcon;
};

