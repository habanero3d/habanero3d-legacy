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

#include "matrix4.h"
#include "VSplit.h"
#include "MFCommon.h"
#include "StaticVertex.h"
#include "GenericGeometry.h"
#include "Mapping.h"
#include "IResource.h"
#include "Frustum.h"

namespace Habanero
{
	template <typename Real> class IBoundingVolume;

	class VDLODStaticMesh : IResource
	{
	public:
		struct InitInfo
		{
			IBoundingVolume<float> *boundingVolume; // TODO: Brzyydkie (wskaznik zmienia wlasciciela)
			GenericGeometry<StaticVertex>::InitInfoExt geometry;
			std::vector<VSplit::InitInfo> vertexSplits;

			std::vector<MF2::VSplitRefinement> refinements;
			std::vector<std::vector<uint>> vertexChildren;
		};
	private:
		VDLODStaticMesh(const VDLODStaticMesh&);	// shielding
		void operator=(const VDLODStaticMesh&);

		uint numVertexSplits;
		VSplit * vertexSplits;	// vertex splits do wzbogacania
		MF2::VSplitRefinement * vsplitRefinements;
		// hierarchia vertexów
		std::vector<uint> activeVSplits;
		std::vector<std::vector<uint>> vsplitChildren;
		std::vector<std::vector<uint>> vsplitParents;

		GenericGeometry<StaticVertex> * geometry;	// podstawowa wersja mesha
		GenericGeometry<StaticVertex> * tmpGeometry;	// pamiêæ na zabawy z geometri¹
		
		std::string filename;
		Resource::Mapping materials, textures;

		bool isOverOrThrough(const plane<float>& plane, const vector3f &center, float radius);
	public:
		const IBoundingVolume<float> * boundingVolume;

		uint getNumVertexSplits() const;

		GenericGeometry<StaticVertex> * getGeometry(const Frustum<float>& fru, const matrix4f &parentToLocal);
		
		VDLODStaticMesh(const std::string &filename,
			Resource::Mapping &materials,
			Resource::Mapping &textures,
			Resource::Mapping &skeletons,
			Resource::Mapping &joints,
			Resource::Mapping &animations);
		~VDLODStaticMesh();

		bool load();
		bool unload();
	};
}
