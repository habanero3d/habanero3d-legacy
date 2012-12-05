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
#pragma once
#include <vector>
#include "SubMesh.h"

namespace Habanero
{
	class GenericGeometryBase
	{
	private:
		SubMesh **subMeshes;	// array of pointers to subMesh
	protected:
		struct InitInfo
		{
			std::vector<SubMesh::InitInfo> subMeshes;
		};

		// brzydkie ale najszybsze co mi przyszło do głowy a śpieszy mi sie...
		struct InitInfoExt
		{
			std::vector<ExtendableSubMesh::InitInfo> subMeshes;
		};

		void *vertexData; // TODO: Czy to powinny byc float'y? (na fixed chyba tylko kolor nie jest)
		size_t vertexDataSize;
		uint numSubMeshes;

		GenericGeometryBase(const GenericGeometryBase::InitInfo &initInfo, const void *vertexData, size_t vertexDataSize);
		GenericGeometryBase(const GenericGeometryBase::InitInfoExt &initInfo, const void *vertexData, size_t vertexDataSize);
		GenericGeometryBase(const GenericGeometryBase::InitInfoExt &initInfo, const void *vertexData, size_t vertexDataSize, bool dontUseExtendable);

	public:
		~GenericGeometryBase();

		const void *getVertexData() const;
		size_t getVertexDataSize() const;
		uint getNumSubMeshes() const;
		SubMesh * getSubMesh(uint index) const;
	};

	template <typename T>
	class GenericGeometry : public GenericGeometryBase
	{
	public:
		struct InitInfo : public GenericGeometryBase::InitInfo
		{
			std::vector<T> vertices;
		};

		// obrzydliwe, j/w
		struct InitInfoExt : public GenericGeometryBase::InitInfoExt
		{
			std::vector<T> vertices;
		};

		GenericGeometry(const InitInfo &initInfo) : GenericGeometryBase(initInfo, &initInfo.vertices[0], initInfo.vertices.size() * sizeof(T))
		{
		}

		GenericGeometry(const InitInfoExt &initInfo) : GenericGeometryBase(initInfo, &initInfo.vertices[0], initInfo.vertices.size() * sizeof(T))
		{
		}

		GenericGeometry(const InitInfoExt &initInfo, bool dontUseExtendable)
			: GenericGeometryBase(initInfo, &initInfo.vertices[0], initInfo.vertices.size() * sizeof(T), dontUseExtendable)
		{
		}

		const T& getVertex(uint index) const
		{
			HASSERT((byte*)vertexData + index < (byte*)vertexData + vertexDataSize);
			return *((T*)vertexData + index);
		}
	};
};
