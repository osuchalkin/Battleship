#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "Constants.h"
#include "Deck.h"

class Ship
{
public:
	Ship(TypeShip type, std::vector<sf::Vector2f>& positions, std::vector<DeckState>& states);
	void render(sf::RenderWindow& window);
	std::vector<sf::Vector2f> getPositions() const { return mPositions; }
	TypeShip getType() const { return mType; }
	void registerHit(const sf::Vector2f& position);
	bool isSunk() const;

private:
	std::vector<std::shared_ptr<Deck>> mDecks;
	TypeShip mType;
	std::vector<sf::Vector2f> mPositions;
	std::vector<DeckState> mDeckStates;
};

