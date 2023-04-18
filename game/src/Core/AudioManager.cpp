#include <irrKlang.h>
#include "Core/AudioManager.h"

using namespace irrklang;

PFA::Core::AudioManager::AudioManager()
{
	m_soundEngine = createIrrKlangDevice();
}

PFA::Core::AudioManager::~AudioManager()
{
	m_soundEngine->drop();
}
const ISoundEngine& PFA::Core::AudioManager::getSoundEngine()
{
	return *m_soundEngine;
}
