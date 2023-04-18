#pragma once
#include <irrKlang.h>

namespace PFA::Core
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


	