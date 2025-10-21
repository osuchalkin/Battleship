#pragma once

#include "Constants.h"
#include <array>
#include <vector>
#include <unordered_set>
#include <map>

using DeckSet = std::unordered_set<int>;
static const std::map<std::string, DeckSet> mDeckSet = {
		{"ShipReady", {4, 7, 10, 12, 14, 16, 17, 18, 19, 20}},
		{"SetA", {0, 4, 7, 10, 12, 14, 16, 17, 18, 19}},
		{"SetB", {1, 5, 8, 11, 13, 15}},
		{"SetC", {2, 3, 6, 9}}
};

class Grid
{
public:
	Grid();
	void setDeck(int x, int y);
	char getCell(int x, int y) const { return mGridCells[x][y]; }
	int getDeckCount() const { return mDeckCount; }	
	bool checkShot(int x, int y);
	void woundedOrKilled(TypeShip type, int x, int y);
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

