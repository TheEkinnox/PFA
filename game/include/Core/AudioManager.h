#pragma once
#include <irrKlang.h>

namespace PFA::CORE
{
	class AudioManager
	{
	public:
		
		AudioManager();
		~AudioManager();
		const irrklang::ISoundEngine& getSoundEngine();

	private:
		irrklang::ISoundEngine* m_soundEngine;
	};
}


	