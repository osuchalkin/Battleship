#include "Ship.h"

Ship::Ship(TypeShip type, std::vector<sf::Vector2f>& positions, std::vector<DeckState>& states) :
	mDecks(),
	mType(type),
	mPositions(positions),
	mDeckStates(states),
	mShipState(ShipState::Alive)

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

void Ship::registerHit(const sf::Vector2f& position)
{
	for (size_t i = 0; i < mPositions.size(); ++i)
	{
		if (mPositions[i] == position)
		{
			mDeckStates[i] = DeckState::Wounded;
			break;
		}
	}
}

bool Ship::isSunk() const
{
	int count = 0;
	for (const auto& state : mDeckStates)
	{
		if (state == DeckState::Wounded)
			count++;
	}

	return count == static_cast<int>(mType);
}