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
	if (mCells[y][x] != EMPTY_CELL)
		return;
	else
		mCells[y][x] = SHIP_CELL;
	mDeckCount++;
}

bool Grid::checkPlaceShip(int x, int y)
{
	mCells[x][y] = SHIP_CELL;
	if (isNumberInSet(mDeckCount, "SetA"))
	{
		return true;
	}
	else if (isNumberInSet(mDeckCount, "SetB"))
	{
		if (mCells[x][y] == mCells[x + 1][y] 
			|| mCells[x][y] == mCells[x - 1][y] 
				|| mCells[x][y] == mCells[x][y + 1]
					|| mCells[x][y] == mCells[x][y - 1])
			return true;
	}
	// TODO
	else if (isNumberInSet(mDeckCount, "SetC"))
	{
		if ((mCells[x][y] == mCells[x + 1][y] && mCells[x][y] == mCells[x + 2][y])
			|| (mCells[x][y] == mCells[x - 1][y] && mCells[x][y] == mCells[x - 2][y])
			|| (mCells[x][y] == mCells[x][y + 1] && mCells[x][y] == mCells[x][y + 2])
			|| (mCells[x][y] == mCells[x][y - 1] && mCells[x][y] == mCells[x][y - 2]))
			return true;
	}
}