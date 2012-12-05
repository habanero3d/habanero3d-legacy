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
@brief Discrete LOD Skinned Mesh Format - implementacja
@author bialy
*/

#include "MTF.h"
#include "File.h"
#include "DLODSMF.h"
#include "Exception.h"
#include "SkinVertex.h"
#include "UnifiedManager.h"

namespace Habanero
{
	namespace DLODSMF
	{
		SkinVertex * DLODSkinnedMesh::getVertices()
		{
			return (SkinVertex*)(this + 1); // tablica vertexów znajduje siê od razu za statycznymi polami
		}

		Habanero::MF2::LODLevel* DLODSkinnedMesh::getFirstLODLevel()
		{
			return (Habanero::MF2::LODLevel*)(getVertices() + numVertices);
		}

		BoundingVolumeType* DLODSkinnedMesh::getBoundingVolumeType()
		{
			Habanero::MF2::LODLevel* lodLevel = getFirstLODLevel();
			for(uint i = 0; i < numLODLevels; i++)
				lodLevel = lodLevel->getNext();
			return (BoundingVolumeType*)lodLevel;
		}


		Habanero::DLODSkinnedMesh::InitInfo loadFromFile(const char *fileName,
															Resource::Mapping &materials,
															Resource::Mapping &textures,
															Resource::Mapping &skeletons)
		{			
			// TODO: burdel, zrobiæ ¿eby dzia³a³o

			uint64 size;
                        // kolejne Kuflowe obejœcie buga gcc
			std::unique_ptr<File, MappingDeleter> file((File*)Habanero::File::map(fileName, Habanero::File::Read, &size).release());
			if (file->signature != signature)
				RAISE(Exception, "Invalid Discrete LOD TMF signature");
			if (size < sizeof(File))
				RAISE(Exception, "Incomplete file");

			Habanero::DLODSkinnedMesh::InitInfo result;

			if (file->mesh.skeletonId != 0)
			{
				result.skeleton = SkeletonManager::getInstance().getResource(skeletons[file->mesh.skeletonId].c_str());
				result.skeleton->load();
			}

			result.dlodLevels.resize(file->mesh.numLODLevels);

			Habanero::MF2::LODLevel * fileLODLevel = file->mesh.getFirstLODLevel();
			for(int i = 0; i < file->mesh.numLODLevels; i++)
			{
				// ka¿dy LODLevel dostaje inn¹ iloœæ vertexów
				DLODLevel<SkinVertex>::InitInfo& lodLevel = result.dlodLevels[i];

				lodLevel.geometry.vertices.resize(fileLODLevel->numVertices);
				std::copy(file->mesh.getVertices(), file->mesh.getVertices() + fileLODLevel->numVertices, lodLevel.geometry.vertices.begin());

				lodLevel.geometry.subMeshes.resize(fileLODLevel->numSubMeshes);
				Habanero::MF2::SubMesh * fileSubMesh = fileLODLevel->getFirstSubMesh();
				for(int j = 0; j < fileLODLevel->numSubMeshes; j++)
				{
					Habanero::SubMesh::InitInfo &subMesh = lodLevel.geometry.subMeshes[j];	// weŸmy no sobie adresik initinfo submesha
					if(fileSubMesh->materialId != 0)
						subMesh.material = shared(MTF2::loadFromFile(materials[fileSubMesh->materialId].c_str(), textures));
					else
						subMesh.material = shared(new Material());
					subMesh.indices.insert(subMesh.indices.begin(), fileSubMesh->indices, fileSubMesh->indices + fileSubMesh->numIndices);

					fileSubMesh = fileSubMesh->getNext();
				}

				fileLODLevel = fileLODLevel->getNext();
			}

			// czytamy BV
			size_t bvSize;
			BoundingVolumeType *bvType = file->mesh.getBoundingVolumeType();
			result.boundingVolume = Habanero::MF::createBoundingVolume(bvType, &bvSize);

			if ((char*)file.get() + size != (char*)bvType + bvSize)
				RAISE(Exception, "Invalid file size");

			return result;
		}
	}
}