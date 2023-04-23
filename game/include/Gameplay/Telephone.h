#ifndef TELEPHONE_H
#define TELEPHONE_H
#include "Component.h"
#include "Core/Color.h"

namespace PFA::Gameplay
{
    class Telephone final : public LibGL::Component
    {
    public:
	    explicit Telephone(LibGL::Entity& owner,
            LibGL::Rendering::Color color = LibGL::Rendering::Color::white);

    private:
        LibGL::Rendering::Color m_color;

	    void updateColor() const;
        void update() override;
    };
}
#endif // TELEPHONE_H
