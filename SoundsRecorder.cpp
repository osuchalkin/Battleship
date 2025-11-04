#include "SoundsRecorder.h"
#include "Constants.h"


SoundsRecorder::SoundsRecorder() :
	mShotBuffer(),
	mWoundedBuffer(),
	mSunkBuffer(),
	mShotSound(),
	mWoundedSound(),
	mSunkSound()
{
	if (!mShotBuffer.loadFromFile(SHOT_SOUND))
		throw std::runtime_error("Failed to load wav-file from data.");
	mShotSound.setBuffer(mShotBuffer);
	if (!mWoundedBuffer.loadFromFile(WOUNDED_SOUND))
		throw std::runtime_error("Failed to load wav-file from data.");
	mWoundedSound.setBuffer(mWoundedBuffer);
	if (!mSunkBuffer.loadFromFile(SUNK_SOUND))
		throw std::runtime_error("Failed to load wav-file from data.");
	mSunkSound.setBuffer(mSunkBuffer);
	
	if (!mSetDeckBuffer.loadFromFile(SET_DECK_SOUND))
		throw std::runtime_error("Failed to load wav-file from data.");
	mSetDeckSound.setBuffer(mSetDeckBuffer);
	
	//if (!mSetShipBuffer.loadFromFile(SET_SHIP_SOUND))
	//	throw std::runtime_error("Failed to load wav-file from data.");
	//mSetShipSound.setBuffer(mSetShipBuffer);
}

void SoundsRecorder::playSound(SoundType type)
{
	switch (type)
	{
	case SoundType::Shot:
		mShotSound.play();
		break;
	case SoundType::Wounded:
		mWoundedSound.play();
		break;
	case SoundType::Sunk:
		mSunkSound.play();
		break;
	case SoundType::SetDeck:
		mSetDeckSound.play();
		break;
	//case SoundType::SetShip:
		//mSetShipSound.play();
	//	break;
	default:
		break;
	}
}
