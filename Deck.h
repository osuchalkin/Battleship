#pragma once

#include <SFML/Graphics.hpp>
#include "Constants.h"

class Deck
{
public:
	Deck(sf::Vector2f& xy, DeckState state);
	void render(sf::RenderWindow& window);

private:
	sf::Vector2f mPosition;
	DeckState mDeckState;
	sf::RectangleShape mDeckShape;
};

