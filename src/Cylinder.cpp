#include "Cylinder.h"

#include <vector>

#include "Memory.h"
#include "ProcessedScene.h"
#include "AABB.h"
#include "HabMath.h"

namespace Habanero
{
	Cylinder::Cylinder(float radius, float height, uint count) 
		:	Spatial(unique(new AABB<float>(-radius, 0, -radius, radius, height, radius))),
			radius(radius),
			height(height),
			count(count)
	{
		generateGeometry();
	}

	void Cylinder::generateGeometry()
	{
		std::vector<Line> lines;
		std::vector<vector3f> bottom, top;
		float angle = 0;
		float angleStep = 2 * pi / count;
		for (uint i = 0; i < count; ++i, angle += angleStep)
		{
			bottom.push_back(vector3f(radius * sin(angle), 0, radius * cos(angle)));
			top.push_back(vector3f(radius * sin(angle), height, radius * cos(angle)));
		}
		for (uint i = 0; i < count; ++i)
		{
			lines.push_back(Line(bottom[i], bottom[(i+1)%count]));
			lines.push_back(Line(top[i], top[(i+1)%count]));
			lines.push_back(Line(bottom[i], top[i]));
		}

		geometry.shaderType = ShaderType::Lines;
		geometry.shaderArgs = shared(new ShaderArgs<Line>(lines.data(), lines.size()));
		geometry.useLights = false;
	}

	void Cylinder::fillScene(const Frustum<float>&, ProcessedScene &scene) const
	{
		GeometryTask task = geometry;
		task.transformation = getWorldToLocalR();
		scene.geometry.push_back(task);
	}

	bool Cylinder::changeRadius(float radius)
	{
		this->radius = radius;
		generateGeometry();
		return true;
	}
}