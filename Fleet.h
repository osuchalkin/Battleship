#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Ship.h"

class Fleet
{
public:
	Fleet();
	void render(sf::RenderWindow& window);
	void createFleetForPreparing();
	void removePreparedShip(); 
	bool mIsFleetReady() { return mShips.empty(); }
private:
	std::vector<std::shared_ptr<Ship>> mShips;
private:
	void createShipForPreparing(TypeShip type, int beginX, int beginY);
	void addShip(std::shared_ptr<Ship> ship) { mShips.push_back(ship); }
	void clear() { mShips.clear(); }
	//void removeShip(std::shared_ptr<Ship> ship) { mShips.erase(std::remove(mShips.begin(), mShips.end(), ship), mShips.end()); }
};

