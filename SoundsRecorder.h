#pragma once

#include <SFML/Audio.hpp>

#include "Constants.h"

class SoundsRecorder
{
public:
	SoundsRecorder();
	// play sounds
	void playSound(SoundType type);
private:
	sf::SoundBuffer mShotBuffer;
	sf::SoundBuffer mWoundedBuffer;
	sf::SoundBuffer mSunkBuffer;
	sf::SoundBuffer mSetDeckBuffer;

	sf::Sound mShotSound;
	sf::Sound mWoundedSound;
	sf::Sound mSunkSound;
	sf::Sound mSetDeckSound;
};

