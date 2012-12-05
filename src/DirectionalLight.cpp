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

#include "DirectionalLight.h"
#include "vector3.h"
#include "Color.h"
#include "Frustum.h"
#include "ProcessedScene.h"
#include "Memory.h"

namespace Habanero
{
	DirectionalLight::DirectionalLight(Color color, vector3f direction, float range) : Light(color, range),
		direction(direction)
	{
	}

	DirectionalLight::~DirectionalLight()
	{
	}

	void DirectionalLight::fillScene(const Frustum<float>&, ProcessedScene &scene) const
	{
		LightTask task;
		task.Type = LightType::DirectionalLight;
		task.color = color;
		
		task.Data.direction = getWorldToLocalR().transformNormal(direction);

		scene.lights.push_back(task);
	}

	void DirectionalLight::initBoundingVolume()
	{
		boundingVolume->growToContain(Range);
	}
}