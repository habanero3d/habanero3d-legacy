#pragma once
#include <memory>

#include "Spatial.h"
#include "vector3.h"
#include "GeometryTask.h"

namespace Habanero
{

	class PlaneGrid : public Spatial
	{
	private:
        GeometryTask geometry;
		typedef std::pair<vector3f, vector3f> Line;

	protected:
		void fillScene(const Frustum<float>&, ProcessedScene &scene) const;
		
	public:
		PlaneGrid(float range, float step);
	};
}