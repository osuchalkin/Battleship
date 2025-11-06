#include "Deck.h"

Deck::Deck(sf::Vector2f& xy, DeckState state) :
	mPosition(xy),
	mDeckState(state),	
	mDeckShape(sf::Vector2f(DECK_SIZE, DECK_SIZE))
{
	mDeckShape.setPosition(mPosition);
}

void Deck::render(sf::RenderWindow& window)

{
	if (mDeckState == DeckState::Normal)
		mDeckShape.setFillColor(DECK_COLOR);
	
	window.draw(mDeckShape);
}
