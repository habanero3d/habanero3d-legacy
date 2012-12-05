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

#include "AmbientLight.h"
#include "vector3.h"
#include "Color.h"
#include "Frustum.h"
#include "ProcessedScene.h"
#include "Memory.h"

namespace Habanero
{
	AmbientLight::AmbientLight(Color color, float range) : Light(color, range)
	{
	}

	void AmbientLight::fillScene(const Frustum<float>&, ProcessedScene &scene) const
	{
		scene.ambient.Red += color.Red;
		scene.ambient.Green += color.Green;
		scene.ambient.Blue += color.Blue;
		scene.ambient.Alpha += color.Alpha;
		if(scene.ambient.Red > 1.) scene.ambient.Red = 1.;
		if(scene.ambient.Green > 1.) scene.ambient.Green = 1.;
		if(scene.ambient.Blue > 1.) scene.ambient.Blue = 1.;
		if(scene.ambient.Alpha > 1.) scene.ambient.Alpha = 1.;
	}

	void AmbientLight::initBoundingVolume()
	{
		boundingVolume->growToContain(Range);
	}
}