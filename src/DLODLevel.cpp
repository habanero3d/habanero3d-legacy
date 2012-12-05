/*
Copyright 2009-2012 Andrzej Skalski, Piotr Kufel, Piotr Bia³ecki, Micha³ ¯ochowski, and Micha³ Szczepaniak
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

#include "DLODLevel.h"
#include "SkinVertex.h"
#include "StaticVertex.h"

namespace Habanero
{
	template <typename V>
	DLODLevel<V>::DLODLevel(const InitInfo& initInfo) :
		geometry(new GenericGeometry<V>(initInfo.geometry))
	{
	}

	template <typename V>
	DLODLevel<V>::~DLODLevel()
	{
		delete geometry;
	}
	
	template <typename V>
	GenericGeometry<V> *DLODLevel<V>::getGeometry() const
	{
		return geometry;
	}

	template <typename V>
	const V& DLODLevel<V>::getVertex(uint index) const
	{
		return geometry->getVertex(index);
	}

	template <typename V>
	const SubMesh * DLODLevel<V>::getSubMesh(uint index) const
	{
		return geometry->getSubMesh(index);
	}

	template class DLODLevel<StaticVertex>;
	template class DLODLevel<SkinVertex>;
}