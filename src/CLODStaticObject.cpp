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

#include "CLODStaticObject.h"
#include "IBoundingVolume.h"
#include "ProcessedScene.h"
#include "HabMath.h"
#include "Memory.h"
#include "Frustum.h"

namespace Habanero
{

	CLODStaticObject::CLODStaticObject(Specific_ptr<CLODStaticMesh> mesh) :
		Spatial(mesh->boundingVolume->clone()),
		mesh(mesh),
		isLODForced(false),
		forcedVSplits(0)
	{
	}

	void CLODStaticObject::initBoundingVolume()
	{
		boundingVolume->growToContain(*mesh->boundingVolume);
	}

	uint CLODStaticObject::computeLODLevel(const Frustum<float>& frustum) const
	{
		if(isLODForced)
			return forcedVSplits;

		HASSERT(cross(frustum.front().normal, frustum.rear().normal).length() == 0);

		float totalDistance = abs(frustum.front().d - frustum.rear().d) / frustum.front().normal.length();

		const matrix4f& worldToLocalR = getWorldToLocalR();
		vector3f translation(worldToLocalR(3, 0), worldToLocalR(3, 1), worldToLocalR(3, 2));

		float objectDistance = abs(dot(frustum.front().normal, translation) + frustum.front().d) / frustum.front().normal.length(); // fajnie się skraca dzięki temu odległość od punktu

		// głupie skalowanie... powinno jakoś wpływać na objectDistance, ale jakoś nie działa więc wyłączam
		//vector3f ox(1, 0, 0);		// zakładam, że uniform scaling (jak nie uniform, to i tak nie bardzo wiem, co robić...)
		//float scaleX = (worldToLocalR * ox).length();
		//objectDistance /= scaleX;

		uint value = mesh->getNumVertexSplits();	// legal values: integers in range [0, value]
		if(value == 0)
			return 0;
	
		if(objectDistance >= totalDistance)
			return 0;

		uint result = value - round((objectDistance / totalDistance) * value);

		return result;
	}

	void CLODStaticObject::fillScene(const Frustum<float>& frustum, ProcessedScene &scene) const
	{
		GeometryTask task;
		task.shaderArgs =  shared<ShaderArgsBase>(NULL);
		task.shaderType = ShaderType::ShaderNone;
		task.transformation = getWorldToLocalR();
		uint lodLevel = computeLODLevel(frustum);
		task.geometry = mesh->getGeometry(lodLevel);
		task.useLights = false;
		scene.geometry.push_back(task);
	}

	const IBoundingVolume<float>* CLODStaticObject::getPickableBoundingVolume() const
	{
		return mesh->boundingVolume;
	}

	bool CLODStaticObject::getIsLODForced() const
	{
		return isLODForced;
	}

	void CLODStaticObject::setIsLODForced(bool value)
	{
		isLODForced = value;
	}

	uint CLODStaticObject::getForcedLevel() const
	{
		return forcedVSplits;
	}

	void CLODStaticObject::setForcedLevel(uint value)
	{
		forcedVSplits = std::max<uint>(0, std::min<uint>(value, mesh->getNumVertexSplits()));
	}

	uint CLODStaticObject::getMaxLevel() const
	{
		return mesh->getNumVertexSplits();
	}
}