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

#pragma once

#include <vector>

#include "VSplit.h"
#include "DLODLevel.h"
#include "StaticVertex.h"
#include "GenericGeometry.h"
#include "Mapping.h"
#include "IResource.h"

namespace Habanero
{
	template <typename Real> class IBoundingVolume;

	class CLODStaticMesh : IResource
	{
	public:
		struct InitInfo
		{
			IBoundingVolume<float> *boundingVolume; // TODO: Brzyydkie (wskaznik zmienia wlasciciela)
			GenericGeometry<StaticVertex>::InitInfoExt geometry;
			std::vector<VSplit::InitInfo> vertexSplits;
		};
	private:
		CLODStaticMesh(const CLODStaticMesh&);	// shielding
		void operator=(const CLODStaticMesh&);

		uint numVertexSplits;
		VSplit * vertexSplits;	// vertex splits do wzbogacania

		GenericGeometry<StaticVertex> * geometry;	// podstawowa wersja mesha
		GenericGeometry<StaticVertex> * tmpGeometry;	// pamiêæ na zabawy z geometri¹
		
		std::string filename;
		Resource::Mapping materials, textures;

		bool isCached;
		uint lastVertexSplits;

		GenericGeometry<StaticVertex> * moveBackward(uint howMuch, std::vector<uint>& counters);
		GenericGeometry<StaticVertex> * moveForward(uint howMuch, std::vector<uint>& counters);
	public:
		const IBoundingVolume<float> * boundingVolume;

		uint getNumVertexSplits() const;

		GenericGeometry<StaticVertex> * getGeometry(uint vertexSplits);
		
		CLODStaticMesh(const std::string &filename,
			Resource::Mapping &materials,
			Resource::Mapping &textures,
			Resource::Mapping &skeletons,
			Resource::Mapping &joints,
			Resource::Mapping &animations);
		~CLODStaticMesh();

		bool load();
		bool unload();
	};
}
