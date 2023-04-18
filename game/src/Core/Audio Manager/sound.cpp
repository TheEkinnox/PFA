#include <irrKlang.h>
#include "Core/Audio Manager/sound.h"

using namespace irrklang;

My::ISoundEngine::ISoundEngine()
{
	m_soundEngine = createIrrKlangDevice();
}

My::ISoundEngine::~ISoundEngine()
{
	m_soundEngine->drop();
}
const ISoundEngine& My::ISoundEngine::getEngine()
{
	return *m_soundEngine;
}
