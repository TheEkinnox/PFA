#pragma once
#include <irrKlang.h>

namespace My
{
	class ISoundEngine
	{
	public:
		
		ISoundEngine();
		~ISoundEngine();
		const irrklang::ISoundEngine& getEngine();

	private:
		irrklang::ISoundEngine* m_soundEngine;
	};
}


	