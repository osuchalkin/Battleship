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
	mIsShipReady(false),
	mGridCells(),
	mDeckCount(0)
{
	for (auto& row : mGridCells)
	{
		row.fill(EMPTY_CELL);
	}
}

void Grid::setDeck(int x, int y)
{
	//mIsShipReady = false;
	if (mGridCells[x][y] != EMPTY_CELL)
		return;
	else if (checkPlaceShip(x, y))
	{
		mGridCells[x][y] = SHIP_CELL;
		mDeckCount++;
		if (isNumberInSet(mDeckCount, "ShipReady"))
		{
			// set HALO
			setHalo(SHIP_CELL);
			mIsShipReady = true;
		}
	}
}

bool Grid::checkPlaceShip(int x, int y)
{
	if (mGridCells[x][y] != EMPTY_CELL)
		return false;

	mGridCells[x][y] = SHIP_CELL;
	if (isNumberInSet(mDeckCount, "SetA"))
	{
		return true;
	}
	else if (isNumberInSet(mDeckCount, "SetB"))
	{
		if (mGridCells[x][y] == mGridCells[x + 1][y] || mGridCells[x][y] == mGridCells[x - 1][y]
			|| mGridCells[x][y] == mGridCells[x][y + 1] || mGridCells[x][y] == mGridCells[x][y - 1])
			return true;
	}
	else if (isNumberInSet(mDeckCount, "SetC"))
	{
		if (x > 1 && x < 10)
		{
			if ((mGridCells[x - 1][y] == SHIP_CELL && mGridCells[x - 2][y] == SHIP_CELL)
				|| (mGridCells[x + 1][y] == SHIP_CELL && mGridCells[x + 2][y] == SHIP_CELL))
				return true;
		}
		if (y > 1 && y < 10)
		{
			if ((mGridCells[x][y - 1] == SHIP_CELL && mGridCells[x][y - 2] == SHIP_CELL)
				|| (mGridCells[x][y + 1] == SHIP_CELL && mGridCells[x][y + 2] == SHIP_CELL))
				return true;
		}
		if (x == 1)
		{
			if (mGridCells[x + 1][y] == SHIP_CELL && mGridCells[x + 2][y] == SHIP_CELL)
				return true;
		}
		if (x == 10)
		{
			if (mGridCells[x - 1][y] == SHIP_CELL && mGridCells[x - 2][y] == SHIP_CELL)
				return true;
		}
		if (y == 1)
		{
			if (mGridCells[x][y + 1] == SHIP_CELL && mGridCells[x][y + 2] == SHIP_CELL)
				return true;
		}
		if (y == 10)
		{
			if (mGridCells[x][y - 1] == SHIP_CELL && mGridCells[x][y - 2] == SHIP_CELL)
				return true;
		}
	}
	// not valid place for ship
	mGridCells[x][y] = EMPTY_CELL;
	return false;
}

void Grid::setHalo(char status)
{
	char status_to_set = (status == SHIP_CELL) ? HALO_CELL : SHOOT_CELL;

	for (int x = 1; x < CELL_WIDTH - 1; x++)
	{
		for (int y = 1; y < CELL_HEIGHT; y++)
		{
			if (mGridCells[x][y] == status)
			{
				if (mGridCells[x - 1][y - 1] == EMPTY_CELL)
					mGridCells[x - 1][y - 1] = status_to_set;
				if (mGridCells[x][y - 1] == EMPTY_CELL)
					mGridCells[x][y - 1] = status_to_set;
				if (mGridCells[x + 1][y - 1] == EMPTY_CELL)
					mGridCells[x + 1][y - 1] = status_to_set;
				if (mGridCells[x - 1][y] == EMPTY_CELL)
					mGridCells[x - 1][y] = status_to_set;
				if (mGridCells[x + 1][y] == EMPTY_CELL)
					mGridCells[x + 1][y] = status_to_set;
				if (mGridCells[x - 1][y + 1] == EMPTY_CELL)
					mGridCells[x - 1][y + 1] = status_to_set;
				if (mGridCells[x][y + 1] == EMPTY_CELL)
					mGridCells[x][y + 1] = status_to_set;
				if (mGridCells[x + 1][y + 1] == EMPTY_CELL)
					mGridCells[x + 1][y + 1] = status_to_set;
			}
		}
	}
}