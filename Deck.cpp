#include "Deck.h"

Deck::Deck(sf::Vector2f& xy, DeckState state) :
	mPosition(xy),
	mDeckState(state),	
	mDeckShape(sf::Vector2f(DECK_SIZE, DECK_SIZE))
{
	mDeckShape.setPosition(mPosition);
	//mDeckShape.setOutlineColor(BCG_COLOR);
	//mDeckShape.setOutlineThickness(1.f);
}

void Deck::render(sf::RenderWindow& window)

{
	if (mDeckState == DeckState::Normal)
		mDeckShape.setFillColor(DECK_COLOR);
	else if (mDeckState == DeckState::Wounded)
		mDeckShape.setFillColor(DECK_WOUNDED_COLOR);
	else if (mDeckState == DeckState::Killed)
		mDeckShape.setFillColor(DECK_KILLED_COLOR);
	
	window.draw(mDeckShape);
}
