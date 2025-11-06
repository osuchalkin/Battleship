#pragma once

#include "PlayerBase.h"
#include <random> 

class Computer :
    public PlayerBase
{
public:
	Computer();
	virtual ~Computer() = default;
	void render(sf::RenderWindow& window) override;
	
	int getRandNum(int first, int last);
	
	void setupFleet();
	void checkShot();

private:
	bool setShip(TypeShip type, int setup);
	sf::Vector2i shooting();
	sf::Vector2i getCellForShot(TypeShip type);
	sf::Vector2i seekShip();
private:
	std::mt19937 mt;
	int mNumberOfMeethods;
	bool mShipWounded;
	std::vector<sf::Vector2i> mWoundedCells;
};

