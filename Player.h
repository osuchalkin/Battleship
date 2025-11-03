#pragma once
#include "PlayerBase.h"

class Player :
    public PlayerBase
{
public:
	Player();
	virtual ~Player() = default;
	void render(sf::RenderWindow& window) override;

	void setupFleet(sf::Vector2i mousePos);
	void checkShot(sf::Vector2i mousePos);
public:
	bool mShipBuild;
};

