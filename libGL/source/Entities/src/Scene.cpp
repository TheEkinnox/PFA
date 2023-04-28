#include "Scene.h"

namespace LibGL::Resources
{
	void Scene::update()
	{
		const auto& nodes = getNodes();

		for (const auto node : nodes)
			node->update();
	}
}
