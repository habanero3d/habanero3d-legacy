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

#pragma once

#include <vector>

#include "DLODLevel.h"
#include "StaticVertex.h"
#include "GenericGeometry.h"
#include "Mapping.h"
#include "IResource.h"

namespace Habanero
{
	template <typename Real> class IBoundingVolume;

	class DLODStaticMesh : IResource
	{
	public:
		struct InitInfo
		{
			IBoundingVolume<float> *boundingVolume; // TODO: Brzyydkie (wskaznik zmienia wlasciciela)
			std::vector<DLODLevel<StaticVertex>::InitInfo> dlodLevels;
		};
	private:
		DLODStaticMesh(const DLODStaticMesh&);	// shielding
		void operator=(const DLODStaticMesh&);
		
		uint numLODLevels;
		DLODLevel<StaticVertex> * lodLevels;
		std::string filename;
		Resource::Mapping materials, textures;

	public:
		//TODO: Prywatyzacja i getter
		IBoundingVolume<float> *boundingVolume;

		uint getNumLODLevels() const;
		GenericGeometry<StaticVertex> *getGeometry(float lodLevel) const;
		
		DLODStaticMesh(const std::string &filename,
			Resource::Mapping &materials,
			Resource::Mapping &textures,
			Resource::Mapping &skeletons,
			Resource::Mapping &joints,
			Resource::Mapping &animations);
		~DLODStaticMesh();

		bool load();
		bool unload();
	};
}
