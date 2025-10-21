#include "Ship.h"

Ship::Ship(TypeShip type, std::vector<sf::Vector2f>& positions, std::vector<DeckState>& states) :
	mDecks(),
	mType(type),
	mPositions(positions),
	mDeckStates(states)

{
	for (int i = 0; i < static_cast<int>(mType); ++i)
	{
		auto deck = std::make_shared<Deck>(mPositions[i], mDeckStates[i]);
		mDecks.push_back(deck);
	}
}

void Ship::render(sf::RenderWindow& window)
{
	for (auto& deck : mDecks)
	{
		deck->render(window);
	}
}