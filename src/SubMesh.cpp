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


/* @file
@author: akuda, qfel
*/

#include <algorithm>

#include "SubMesh.h"

namespace Habanero
{
	SubMesh::SubMesh(const SubMesh::InitInfo &initInfo) :
		numIndices(initInfo.indices.size()),
		material(initInfo.material)
	{
		indices = new uint[numIndices];
		std::copy(initInfo.indices.begin(), initInfo.indices.end(), indices);
	}

	SubMesh::SubMesh()
	{
	}

	SubMesh::~SubMesh()
	{
		delete[] indices;
	}

	uint SubMesh::getNumIndices() const
	{
		return numIndices;
	}

	std::shared_ptr<Material> SubMesh::getMaterial() const
	{
		return material;
	}

	uint SubMesh::getIndex(uint index) const
	{
		HASSERT(index < numIndices);
		return indices[index];
	}

	uint * SubMesh::getIndices() const
	{
		return indices;
	}
	
	ExtendableSubMesh::ExtendableSubMesh(const ExtendableSubMesh::InitInfo& initInfo)
		: maxIndices(initInfo.maxIndices)
	{
		numIndices = initInfo.indices.size();
		material = initInfo.material;

		indices = new uint[maxIndices];
		std::copy(initInfo.indices.begin(), initInfo.indices.end(), indices);
	}

	void ExtendableSubMesh::setIndex(int index, uint value)
	{
		HASSERT(index < maxIndices);
		indices[index] = value;
	}

	void ExtendableSubMesh::setNumIndices(uint newNumIndices)
	{
		numIndices = newNumIndices;
	}

	ExtendableSubMesh::~ExtendableSubMesh()
	{
	}
}