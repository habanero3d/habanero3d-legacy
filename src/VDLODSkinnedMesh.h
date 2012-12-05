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


/* @file
@author: Bia�y
*/

#pragma once
#include <vector>

#include "GenericGeometry.h"
#include "MFCommon.h"
#include "SkinVertex.h"
#include "Skeleton.h"
#include "SubMesh.h"
#include "Mapping.h"
#include "IResource.h"
#include "VSplit.h"
#include "Frustum.h"

namespace Habanero
{
	template <typename Real> class IBoundingVolume;

	class VDLODSkinnedMesh : IResource
	{
	public:
		struct InitInfo
		{
			IBoundingVolume<float> * boundingVolume; // TODO: Brzyydkie (wskaznik zmienia wlasciciela)
			Specific_ptr<Skeleton> skeleton;
			GenericGeometry<SkinVertex>::InitInfoExt geometry;
			std::vector<VSplit::InitInfo> vertexSplits;

			std::vector<MF2::VSplitRefinement> refinements;
			std::vector<std::vector<uint>> vertexChildren;
		};

	private:
		VDLODSkinnedMesh(const VDLODSkinnedMesh&);
		void operator =(const VDLODSkinnedMesh&);

		uint numVertexSplits;
		VSplit * vertexSplits;
		MF2::VSplitRefinement * vsplitRefinements;
		// hierarchia vertex�w
		std::vector<uint> activeVSplits;
		std::vector<std::vector<uint>> vsplitChildren;
		std::vector<std::vector<uint>> vsplitParents;

		GenericGeometry<SkinVertex> * geometry;	// podstawowa wersja mesha
		GenericGeometry<SkinVertex> * tmpGeometry;	// pami�� na zabawy z geometri�

		std::string filename;
		Resource::Mapping materials, textures, skeletons;

		bool isOverOrThrough(const plane<float>& plane, const vector3f &center, float radius);
	public:
		const IBoundingVolume<float> * boundingVolume;
		Specific_ptr<Skeleton> skeleton;

		uint getNumVertexSplits() const;

		GenericGeometry<SkinVertex> * getGeometry(const Frustum<float>& fru, const matrix4f &parentToLocal);

		VDLODSkinnedMesh(const std::string &filename,
			Resource::Mapping &materials,
			Resource::Mapping &textures,
			Resource::Mapping &skeletons,
			Resource::Mapping &joints,
			Resource::Mapping &animations);
		~VDLODSkinnedMesh();

		bool load();
		bool unload();
	};
}
