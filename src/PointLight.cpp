/*
Copyright 2009-2012 Andrzej Skalski, Piotr Kufel, Piotr Białecki, Michał Żochowski, and Michał Szczepaniak
This file is part of Habanero3d.

Habanero3d is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Habanero3d is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Habanero3d.  If not, see <http://www.gnu.org/licenses/>.


*/


/*author: akuda */

#include "PointLight.h"
#include "vector3.h"
#include "Color.h"
#include "Frustum.h"
#include "ProcessedScene.h"
#include "Memory.h"

namespace Habanero
{
	PointLight::PointLight(Color color, float range) : Light(color, range)
	{
	}

	PointLight::~PointLight()
	{
	}

	void PointLight::fillScene(const Frustum<float>&, ProcessedScene &scene) const
	{
		LightTask task;
		task.Type = LightType::PointLight;
		task.color = color;
		
		task.Data.radius = getWorldToLocalR().transformNormal(vector3f(Range.radius, 0, 0)).length();
		task.Data.position = this->getWorldToLocalR().transformVertex(vector3f::zero);

		scene.lights.push_back(task);
	}

	void PointLight::initBoundingVolume()
	{
		boundingVolume->growToContain(Range);
	}
}