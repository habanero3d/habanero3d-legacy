#include "PlaneGrid.h"

#include <vector>

#include "Memory.h"
#include "ProcessedScene.h"
#include "AABB.h"

namespace Habanero
{
	PlaneGrid::PlaneGrid(float range, float step) : Spatial(unique(new AABB<float>(range, 0., range, -range, 0., -range)))
	{
		std::vector<Line> lines;
		for (float coord = 0; coord < range; coord += step)
		{
			lines.push_back(Line(vector3f(-range, 0., coord), vector3f(range, 0., coord)));
			lines.push_back(Line(vector3f(coord, 0., -range), vector3f(coord, 0., range)));
			lines.push_back(Line(vector3f(-range, 0., -coord), vector3f(range, 0., -coord)));
			lines.push_back(Line(vector3f(-coord, 0., -range), vector3f(-coord, 0., range)));
		}

		geometry.shaderType = ShaderType::Lines;
		geometry.shaderArgs = shared(new ShaderArgs<Line>(lines.data(), lines.size()));
		geometry.useLights = false;
	}

	void PlaneGrid::fillScene(const Frustum<float>&, ProcessedScene &scene) const
	{
		GeometryTask task = geometry;
		task.transformation = getWorldToLocalR();
		scene.geometry.push_back(task);
	}
}