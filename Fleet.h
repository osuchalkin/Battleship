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
	//bool IsFleetPrepareReady() { return mShips.empty(); }
	void createShip(std::vector<sf::Vector2i>& positions);
	//TypeShip getTypeShip(sf::Vector2i position);
	Ship* getShipAtPosition(sf::Vector2i position);
	void updateFleet();
	bool isFleetDead() { return mShips.empty(); };
	//void clearFleet() { mShips.clear(); }
private:
	std::vector<std::shared_ptr<Ship>> mShips;
private:
	void createShipForPreparing(TypeShip type, int beginX, int beginY);
	void addShip(std::shared_ptr<Ship> ship) { mShips.push_back(ship); }
	//void removeShip(std::shared_ptr<Ship> ship) { mShips.erase(std::remove(mShips.begin(), mShips.end(), ship), mShips.end()); }
};

