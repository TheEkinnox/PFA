#pragma once
#include <irrKlang.h>

namespace PFA::Core
{
	class AudioManager
	{
	public:
		
		AudioManager();
		~AudioManager();
		irrklang::ISoundEngine& getSoundEngine() const;

	private:
		irrklang::ISoundEngine* m_soundEngine;
	};
}


	