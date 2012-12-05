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


/*!
@file
@brief Discrete LOD Skinned Mesh Format
@author bialy
*/

#include "MFCommon.h"
#include "DLODSkinnedMesh.h"

namespace Habanero
{
	namespace DLODSMF
	{
		const uint signature = '1FSD';

		struct DLODSkinnedMesh
		{
			uint skeletonId;
			uint numVertices;
			uint numLODLevels;

			SkinVertex* getVertices();
			Habanero::MF2::LODLevel* getFirstLODLevel();
			BoundingVolumeType* getBoundingVolumeType();
		};

		struct File
		{
			uint signature;
			DLODSkinnedMesh mesh;
		};

		Habanero::DLODSkinnedMesh::InitInfo loadFromFile(const char * fileName,
			Resource::Mapping &materials,
			Resource::Mapping &textures,
			Resource::Mapping &skeletons);
	}
}