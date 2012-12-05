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
#include "SkinVertex.h"
#include "Skeleton.h"
#include "SubMesh.h"
#include "Mapping.h"
#include "IResource.h"
#include "VSplit.h"

namespace Habanero
{
	template <typename Real> class IBoundingVolume;

	class CLODSkinnedMesh : IResource
	{
	public:
		struct InitInfo
		{
			IBoundingVolume<float> * boundingVolume; // TODO: Brzyydkie (wskaznik zmienia wlasciciela)
			Specific_ptr<Skeleton>skeleton;
			GenericGeometry<SkinVertex>::InitInfoExt geometry;
			std::vector<VSplit::InitInfo> vertexSplits;
		};

	private:
		CLODSkinnedMesh(const CLODSkinnedMesh&);
		void operator =(const CLODSkinnedMesh&);

		uint numVertexSplits;
		VSplit * vertexSplits;

		GenericGeometry<SkinVertex> * geometry;	// podstawowa wersja mesha
		GenericGeometry<SkinVertex> * tmpGeometry;	// pami�� na zabawy z geometri�

		std::string filename;
		Resource::Mapping materials, textures, skeletons;

		bool isCached;
		uint lastVertexSplits;

		GenericGeometry<SkinVertex> * moveForward(uint howMuch, std::vector<uint>& counters);
		GenericGeometry<SkinVertex> * moveBackward(uint howMuch, std::vector<uint>& counters);
	public:
		const IBoundingVolume<float> * boundingVolume;
		Specific_ptr<Skeleton> skeleton;

		uint getNumVertexSplits() const;

		GenericGeometry<SkinVertex> * getGeometry(uint vertexSplits);

		CLODSkinnedMesh(const std::string &filename,
			Resource::Mapping &materials,
			Resource::Mapping &textures,
			Resource::Mapping &skeletons,
			Resource::Mapping &joints,
			Resource::Mapping &animations);
		~CLODSkinnedMesh();

		bool load();
		bool unload();
	};
}
