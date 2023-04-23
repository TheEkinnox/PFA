#ifndef CUBE_H
#define CUBE_H
#include "Component.h"

namespace LibGL::Rendering
{
	struct Color;
}

namespace PFA::Gameplay
{
	class Cube final : public LibGL::Component
	{
	public:
		explicit Cube(LibGL::Entity& owner);

		void swapColors(LibGL::Rendering::Color& otherColor) const;
	};
}
#endif // CUBE_H
