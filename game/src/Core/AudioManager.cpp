#include <irrKlang.h>
#include "Core/AudioManager.h"

using namespace irrklang;

PFA::CORE::AudioManager::AudioManager()
{
	m_soundEngine = createIrrKlangDevice();
}

PFA::CORE::AudioManager::~AudioManager()
{
	m_soundEngine->drop();
}
const ISoundEngine& PFA::CORE::AudioManager::getSoundEngine()
{
	return *m_soundEngine;
}
