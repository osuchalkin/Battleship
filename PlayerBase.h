#pragma once

#include <SFML/Graphics.hpp>
#include <array>

#include "Constants.h"
#include "Grid.h"
#include "Fleet.h"

using CellsArray = std::array<std::array<sf::RectangleShape, CELL_WIDTH>, CELL_HEIGHT>;

class PlayerBase
{
public:
	PlayerBase();
	virtual ~PlayerBase() = default;
	virtual void render(sf::RenderWindow& window);

	// getters
	const std::shared_ptr<Grid>& getGridPtr() const { return mGrid; }
	const std::shared_ptr<Fleet>& getFleetPtr() const { return mFleet; }
	const CellsArray& getCells() const { return mCells; }
	// setters
	void setupEnemyData(
		Grid* enemyGrid,
		Fleet* enemyFleet,
		const CellsArray* enemyCells)
	{
		mEnemyGrid = enemyGrid;
		mEnemyFleet = enemyFleet;
		mEnemyCells = enemyCells;
	}
	bool isFleetSank() { return mFleet->isFleetDead(); }
	void setSound(bool soundOn);
public:
	bool mMissedTarget;

protected:
	float lineLength = 50.0f;
	float offset = lineLength / 2.0f * (float)std::cos(45.0f * PI / 180.0f);

	sf::CircleShape mShotIndicator;
	sf::VertexArray mCross;

	CellsArray mCells;
	std::shared_ptr<Grid> mGrid;
	std::shared_ptr<Fleet> mFleet;

	Grid* mEnemyGrid;
	Fleet* mEnemyFleet;
	const CellsArray* mEnemyCells;
protected:
	void setupCells(int margin);
	void renderCells(sf::RenderWindow& window, int margin);
	void renderShips(sf::RenderWindow& window, int margin);
};
