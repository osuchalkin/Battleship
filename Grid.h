#pragma once

#include "Constants.h"
#include "Ship.h"

#include <array>
#include <vector>
#include <unordered_set>
#include <map>
#include <functional>

using DeckSet = std::unordered_set<int>;
static const std::map<std::string, DeckSet> mDeckSet = {
		{"ShipReady", {4, 7, 10, 12, 14, 16, 17, 18, 19, 20}},
		{"SetA", {0, 4, 7, 10, 12, 14, 16, 17, 18, 19}},
		{"SetB", {1, 5, 8, 11, 13, 15}},
		{"SetC", {2, 3, 6, 9}}
};

using ConditionPredicate = std::function<bool(int col, int row)>;

class Grid
{
public:
	Grid();
	void setDeck(int x, int y);
	void setShipPositions(const std::vector<sf::Vector2i>& positions);
	char getCell(int x, int y) const { return mGridCells[x][y]; }
	void setCell(int x, int y, char status) { mGridCells[x][y] = status; }
	int getDeckCount() const { return mDeckCount; }	
	bool checkShot(int x, int y);
	void woundedOrKilled(Ship* ship, int x, int y);
	//sf::Vector2i getRandomPositionForShot();
	std::vector <std::vector<sf::Vector2i>> getValidPositions(ConditionPredicate checkCondition, int ship,
		int colStart = 1, int colFinish = CELL_WIDTH - 1,
		int rowStart = 1, int rowFinish = CELL_HEIGHT - 1);
public:
	bool mIsShipReady;
	std::vector<sf::Vector2i> mPositions;
private:
	std::array<std::array<char, CELL_WIDTH>, CELL_HEIGHT> mGridCells;
	int mDeckCount;
private:
	bool checkPlaceShip(int x, int y);
	void setHalo(char status);
};

