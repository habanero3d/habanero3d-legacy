#pragma once
#include <memory>

#include "Spatial.h"
#include "vector3.h"
#include "GeometryTask.h"

namespace Habanero
{

	class Cylinder : public Spatial
	{
	private:
        GeometryTask geometry;
		typedef std::pair<vector3f, vector3f> Line;
		float radius;
		float height;
		uint count;
		void generateGeometry();

	protected:
		void fillScene(const Frustum<float>&, ProcessedScene &scene) const;
		
	public:
		Cylinder(float radius, float height, uint count);
		bool changeRadius(float radius);
	};
}