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

#include <stddef.h>
#include <math.h>

#include "HeightMapObject.h"
#include "AABB.h"
#include "Memory.h"
#include "Spatial.h"
#include "GeometryTask.h"
#include "StaticVertex.h"
#include "ProcessedScene.h"
#include "GenericGeometry.h"
#include "Memory.h"
#include "HabMath.h"

namespace Habanero
{
	HeightMapObject::HeightMapObject(Specific_ptr<HeightMapMesh> hm) :
		Spatial(hm->boundingVolume->clone()), heightMap(hm)
	{
	}

	void HeightMapObject::initBoundingVolume()
	{
		boundingVolume->growToContain(*(heightMap->boundingVolume));
	}

	void HeightMapObject::fillScene(const Frustum<float> &fru, ProcessedScene &scene) const
	{
		GeometryTask task;
		task.shaderArgs = shared<ShaderArgsBase>(NULL);
		task.shaderType = ShaderType::ShaderNone;
		task.geometry = heightMap->getGeometry();
		task.useLights = false;
		task.transformation = getWorldToLocalR();
		scene.geometry.push_back(task);
	}

	//float HeightMap::getHeightAt(float x, float y) const
    float HeightMapObject::getHeightAt(double x, double y) const
	{
		return heightMap->getHeightAt(x, y);
	}

	bool HeightMapObject::reload()
	{
		Specific_ptr<Texture> tex = TextureManager::getInstance().getResource(heightMap->getTerrainTexture());
		tex->unload();
		heightMap->unload();
		return heightMap->load();
	}
}
