#include "Grid.h"

#include <iostream>		// for debug	

static bool isNumberInSet(int number, const std::string& set_name) {
	// Находим нужный набор по имени
	auto it = mDeckSet.find(set_name);

	if (it == mDeckSet.end())
		return false;

	// Выполняем быструю O(1) проверку
	return it->second.count(number);
}


Grid::Grid() :
	mCells(),
	mDeckCount(0)
{
	for (auto& row : mCells)
	{
		row.fill(EMPTY_CELL);
	}
}

void Grid::setDeck(int x, int y)
{
	std::cout << "Check place ship at (" << x << ", " << y << ")" << std::endl;
	if (mCells[y][x] != EMPTY_CELL)
		return;
	else if (checkPlaceShip(y, x))
	{
		mCells[y][x] = SHIP_CELL;
		mDeckCount++;
		if (isNumberInSet(mDeckCount, "ShipReady"))
		{
			// set HALO
			std::cout << "Ship is ready!" << std::endl;
		}
	}
}

bool Grid::checkPlaceShip(int x, int y)
{
	if (mCells[x][y] != EMPTY_CELL)
		return false;

	mCells[x][y] = SHIP_CELL;
	if (isNumberInSet(mDeckCount, "SetA"))
	{
		return true;
	}
	else if (isNumberInSet(mDeckCount, "SetB"))
	{
		if (mCells[x][y] == mCells[x + 1][y] || mCells[x][y] == mCells[x - 1][y] 
			|| mCells[x][y] == mCells[x][y + 1] || mCells[x][y] == mCells[x][y - 1])
			return true;
	}
	else if (isNumberInSet(mDeckCount, "SetC"))
	{
		if (1 < x < 10)
		{
			if (mCells[x - 1][y] == mCells[x - 2][y] == SHIP_CELL 
				|| mCells[x + 1][y] == mCells[x + 2][y] == SHIP_CELL)
				return true;
		}
		else if (1 < y < 10)
		{
			if (mCells[x][y - 1] == mCells[x][y - 2] == SHIP_CELL
				|| mCells[x][y + 1] == mCells[x][y + 2] == SHIP_CELL)
				return true;
		}
		else if (x == 1)
		{
			if (mCells[x + 1][y] == mCells[x + 2][y] == SHIP_CELL)
				return true;
		}
		else if (x == 10)
		{
			if (mCells[x - 1][y] == mCells[x - 2][y] == SHIP_CELL)
				return true;
		}
		else if (y == 1)
		{
			if (mCells[x][y + 1] == mCells[x][y + 2] == SHIP_CELL)
				return true;
		}
		else if (y == 10)
		{
			if (mCells[x][y - 1] == mCells[x][y - 2] == SHIP_CELL)
				return true;
		}
	}
	else
	{
		mCells[x][y] = EMPTY_CELL;
		return false;
	}	
}