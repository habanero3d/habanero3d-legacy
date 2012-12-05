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


/*!
@file
@brief Wspólna postać dla obiektów geometrycznych.
@author qfel
*/
#include <string.h>
#include <memory.h>

#include "GenericGeometry.h"
#include "Memory.h"

namespace Habanero
{
	GenericGeometryBase::GenericGeometryBase(const GenericGeometryBase::InitInfo &initInfo, const void *vertexData, size_t vertexDataSize) :
		vertexDataSize(vertexDataSize),
		numSubMeshes(initInfo.subMeshes.size())
	{
		this->vertexData = new char[vertexDataSize];
		memcpy(this->vertexData, vertexData, vertexDataSize);

		subMeshes = new SubMesh*[numSubMeshes];
		for (uint i = 0; i < initInfo.subMeshes.size(); i++)
			subMeshes[i] = new SubMesh(initInfo.subMeshes[i]);
	}

	GenericGeometryBase::GenericGeometryBase(const GenericGeometryBase::InitInfoExt &initInfo, const void *vertexData, size_t vertexDataSize) :
		vertexDataSize(vertexDataSize),
		numSubMeshes(initInfo.subMeshes.size())
	{
		this->vertexData = new char[vertexDataSize];
		memcpy(this->vertexData, vertexData, vertexDataSize);

		subMeshes = new SubMesh*[numSubMeshes];
		for (uint i = 0; i < initInfo.subMeshes.size(); i++)
			subMeshes[i] = new ExtendableSubMesh(initInfo.subMeshes[i]);
	}

	GenericGeometryBase::GenericGeometryBase(const GenericGeometryBase::InitInfoExt &initInfo, const void *vertexData, size_t vertexDataSize, bool dontUseExtendable)
		: vertexDataSize(vertexDataSize),
		numSubMeshes(initInfo.subMeshes.size())
	{
		this->vertexData = new char[vertexDataSize];
		memcpy(this->vertexData, vertexData, vertexDataSize);

		subMeshes = new SubMesh*[numSubMeshes];
		for (uint i = 0; i < initInfo.subMeshes.size(); i++)
			subMeshes[i] = new SubMesh(initInfo.subMeshes[i]);
	}

	GenericGeometryBase::~GenericGeometryBase()
	{
		delete[] (char*)vertexData;
		for(int i = 0; i < numSubMeshes; i++)
			delete subMeshes[i];
		delete [] subMeshes;
		//uninitializedDelete(subMeshes, numSubMeshes);
	}

	const void *GenericGeometryBase::getVertexData() const
	{
		return vertexData;
	}

	size_t GenericGeometryBase::getVertexDataSize() const
	{
		return vertexDataSize;
	}

	uint GenericGeometryBase::getNumSubMeshes() const
	{
		return numSubMeshes;
	}

	SubMesh * GenericGeometryBase::getSubMesh(uint index) const
	{
		HASSERT(index < numSubMeshes);
		return subMeshes[index];
	}
};