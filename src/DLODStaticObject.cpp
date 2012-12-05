/*
Copyright 2009-2012 Andrzej Skalski, Piotr Kufel, Piotr Bia�ecki, Micha� �ochowski, and Micha� Szczepaniak
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

/*! @file
@author bialy
*/

#include "DLODStaticObject.h"
#include "IBoundingVolume.h"
#include "ProcessedScene.h"
#include "HabMath.h"
#include "Memory.h"
#include "Frustum.h"

namespace Habanero
{

	DLODStaticObject::DLODStaticObject(Specific_ptr<DLODStaticMesh> mesh) :
		Spatial(mesh->boundingVolume->clone()),
		mesh(mesh),
		isLODForced(false),
		forcedLevel(0)
	{
	}

	void DLODStaticObject::initBoundingVolume()
	{
		boundingVolume->growToContain(*mesh->boundingVolume);
	}


	float DLODStaticObject::computeLODLevel(const Frustum<float>& frustum) const
	{
		if(isLODForced)
			return forcedLevel;

		HASSERT(cross(frustum.front().normal, frustum.rear().normal).length() == 0);
		HASSERT(frustum.front().normal.length() == 1);

		float totalDistance = abs(frustum.front().d - frustum.rear().d) / frustum.front().normal.length();

		const matrix4f& worldToLocalR = getWorldToLocalR();
		vector3f translation(worldToLocalR(3, 0), worldToLocalR(3, 1), worldToLocalR(3, 2));

		float objectDistance = abs(dot(frustum.front().normal, translation) + frustum.front().d) / frustum.front().normal.length(); // fajnie się skraca dzięki temu odległość od punktu

		// głupie skalowanie... powinno jakoś wpływać na objectDistance, ale średnio działa, więc wyłączam
		//vector3f ox(1, 0, 0);		// zakładam, że uniform scaling (jak nie uniform, to i tak nie bardzo wiem, co robić...)
		//float scaleX = (worldToLocalR * ox).length();
		//objectDistance /= scaleX;

		uint maxValue = mesh->getNumLODLevels() - 1;	// max possible value
		if(maxValue == 0)
			return 0;
		if(objectDistance > totalDistance)
			return maxValue;
		return floor(maxValue * objectDistance / totalDistance);
	}

	void DLODStaticObject::fillScene(const Frustum<float>& frustum, ProcessedScene &scene) const
	{
		GeometryTask task;
		task.shaderArgs =  shared<ShaderArgsBase>(NULL);
		task.shaderType = ShaderType::ShaderNone;
		task.transformation = getWorldToLocalR();
		float lodLevel = computeLODLevel(frustum);
		task.geometry = mesh->getGeometry(lodLevel);
		task.useLights = false;
		scene.geometry.push_back(task);
	}

	const IBoundingVolume<float>* DLODStaticObject::getPickableBoundingVolume() const
	{
		return mesh->boundingVolume;
	}

	bool DLODStaticObject::getIsLODForced() const
	{
		return isLODForced;
	}

	void DLODStaticObject::setIsLODForced(bool value)
	{
		isLODForced = value;
	}

	uint DLODStaticObject::getForcedLevel() const
	{
		return forcedLevel;
	}

	void DLODStaticObject::setForcedLevel(uint value)
	{
		value = clamp<uint>(value, 0, mesh->getNumLODLevels() - 1);
		forcedLevel = value;
	}
}