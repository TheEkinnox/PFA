#include "Gameplay/Cube.h"
#include "Entity.h"

#include "Debug/Assertion.h"
#include "LowRenderer/Mesh.h"

using namespace LibMath;
using namespace LibGL;
using namespace LibGL::Rendering;

namespace PFA::Gameplay
{
    Cube::Cube(Entity& owner) : Component(owner)
    {
        ASSERT(typeid(owner) == typeid(Mesh) || dynamic_cast<Mesh*>(&owner) != nullptr);
    }

    void Cube::swapColors(Color& otherColor) const
    {
        Entity& owner = getOwner();
        ASSERT(typeid(owner) == typeid(Mesh) || dynamic_cast<Mesh*>(&owner) != nullptr);

        Mesh& ownerMesh = dynamic_cast<Mesh&>(owner);
        const Color tmp = ownerMesh.getMaterial().getTint();
        ownerMesh.getMaterial().setTint(otherColor);
        otherColor = tmp;
    }
}