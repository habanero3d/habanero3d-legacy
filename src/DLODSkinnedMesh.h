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

#include "Mapping.h"
#include "Skeleton.h"
#include "DLODLevel.h"
#include "SkinVertex.h"
#include "IResource.h"

namespace Habanero
{
	template <typename Real> class IBoundingVolume;

	class DLODSkinnedMesh : IResource
	{
	public:
		struct InitInfo
		{
			IBoundingVolume<float> *boundingVolume; // TODO: Brzyydkie (wskaznik zmienia wlasciciela)
			std::vector<DLODLevel<SkinVertex>::InitInfo> dlodLevels;
			Specific_ptr<Skeleton> skeleton;
		};

	private:
		DLODSkinnedMesh(const DLODSkinnedMesh&);	// shielding
		void operator=(const DLODSkinnedMesh&);
		
		uint numLODLevels;
		DLODLevel<SkinVertex> * lodLevels;

		std::string filename;
		Resource::Mapping materials, textures, skeletons;

	public:
		const IBoundingVolume<float> * boundingVolume;
		Specific_ptr<Skeleton> skeleton;

		uint getNumLODLevels() const;
		GenericGeometry<SkinVertex> *getGeometry(float lodLevel) const;
		
		DLODSkinnedMesh(const std::string &filename,
			Resource::Mapping &materials,
			Resource::Mapping &textures,
			Resource::Mapping &skeletons,
			Resource::Mapping &joints,
			Resource::Mapping &animations);
		~DLODSkinnedMesh();

		bool load();
		bool unload();
	};
}
