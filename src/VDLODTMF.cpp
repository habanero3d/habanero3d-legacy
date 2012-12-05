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

#include "VDLODTMF.h"
#include "VSplit.h"
#include "MFCommon.h"
#include "StaticVertex.h"
#include "Exception.h"
#include "File.h"
#include "MTF.h"

namespace Habanero
{
	namespace VDLODTMF
	{
		StaticVertex* VDLODStaticMesh::getVertices()
		{
			return (StaticVertex*)(this + 1); // tablica vertexów znajduje siê od razu za statycznymi polami
		}

		Habanero::MF2::SubMesh* VDLODStaticMesh::getFirstSubMesh()
		{
			return (Habanero::MF2::SubMesh*)(getVertices() + numVertices);
		}

		Habanero::uint * VDLODStaticMesh::getFirstMaxIndex()
		{
			Habanero::MF2::SubMesh *subMesh = getFirstSubMesh();
			for (uint i = 0; i < numSubMeshes; i++)
				subMesh = subMesh->getNext();
			return (uint *)subMesh;
		}

		Habanero::MF2::VSplit* VDLODStaticMesh::getFirstVSplit()
		{
			return (Habanero::MF2::VSplit *)(getFirstMaxIndex() + numSubMeshes);			
		}

		Habanero::MF2::VSplitRefinement * VDLODStaticMesh::getFirstRefinement()
		{
			Habanero::MF2::VSplit * vSplit = getFirstVSplit();
			for(uint i = 0; i < numVertexSplits; i++)
				vSplit = vSplit->getNext();
			return (MF2::VSplitRefinement*)vSplit;
		}

		uint * VDLODStaticMesh::getFirstChildrenLength()
		{
			return (uint *)(getFirstRefinement() + numVertexSplits);
		}

		uint * VDLODStaticMesh::getFirstChildren()
		{
			return (uint *)(getFirstChildrenLength() + numVertexSplits);
		}

		BoundingVolumeType* VDLODStaticMesh::getBoundingVolumeType()
		{
			uint * childLengths = getFirstChildrenLength();

			uint * children = getFirstChildren();

			for(uint i = 0; i < numVertexSplits; i++)
			{
				children += *childLengths;
				childLengths++;
			}

			return (BoundingVolumeType *)children;
		}

		Habanero::VDLODStaticMesh::InitInfo loadFromFile(const char *fileName, Resource::Mapping &materials, Resource::Mapping &textures)
		{			
			// TODO: burdel
			uint64 size;
                        // kolejne Kuflowe obejœcie buga gcc
			std::unique_ptr<File, MappingDeleter> file((File*)Habanero::File::map(fileName, Habanero::File::Read, &size).release());
			if (file->signature != signature)
				RAISE(Exception, "Invalid Discrete LOD TMF signature");
			if (size < sizeof(File))
				RAISE(Exception, "Incomplete file");

			Habanero::VDLODStaticMesh::InitInfo vdlodStaticMesh;

			vdlodStaticMesh.geometry.vertices.resize(file->mesh.numVertices);
			std::copy(file->mesh.getVertices(), file->mesh.getVertices() + file->mesh.numVertices, vdlodStaticMesh.geometry.vertices.begin());

			vdlodStaticMesh.geometry.subMeshes.resize(file->mesh.numSubMeshes);
			Habanero::MF2::SubMesh * fileSubMesh = file->mesh.getFirstSubMesh();
			for(int j = 0; j < file->mesh.numSubMeshes; j++)
			{
				Habanero::ExtendableSubMesh::InitInfo &subMesh = vdlodStaticMesh.geometry.subMeshes[j];	// weŸmy no sobie adresik initinfo submesha
				subMesh.maxIndices = *(file->mesh.getFirstMaxIndex() + j);
				if(fileSubMesh->materialId != 0)
					subMesh.material = shared(MTF2::loadFromFile(materials[fileSubMesh->materialId].c_str(), textures));
				else
					subMesh.material = shared((Material*)NULL);
				subMesh.indices.insert(subMesh.indices.begin(), fileSubMesh->indices, fileSubMesh->indices + fileSubMesh->numIndices);

				fileSubMesh = fileSubMesh->getNext();
			}

			vdlodStaticMesh.vertexSplits.resize(file->mesh.numVertexSplits);
			Habanero::MF2::VSplit * vSplit = file->mesh.getFirstVSplit();
			for(int i = 0; i < file->mesh.numVertexSplits; i++)
			{
				vdlodStaticMesh.vertexSplits[i].subMeshId = vSplit->subMeshId;

				vdlodStaticMesh.vertexSplits[i].moves.resize(vSplit->numMoves);
				std::copy(vSplit->getFirstMove(), vSplit->getFirstMove() + vSplit->numMoves, vdlodStaticMesh.vertexSplits[i].moves.begin());

				vdlodStaticMesh.vertexSplits[i].additions.resize(vSplit->numAdditions);
				std::copy(vSplit->getFirstAddition(), vSplit->getFirstAddition() + vSplit->numAdditions, vdlodStaticMesh.vertexSplits[i].additions.begin());

				vSplit = vSplit->getNext();
			}

			vdlodStaticMesh.refinements.resize(file->mesh.numVertexSplits);
			std::copy(file->mesh.getFirstRefinement(), file->mesh.getFirstRefinement() + file->mesh.numVertexSplits, vdlodStaticMesh.refinements.begin());

			vdlodStaticMesh.vertexChildren.resize(file->mesh.numVertexSplits);
			for(uint i = 0; i < file->mesh.numVertexSplits; i++)
				vdlodStaticMesh.vertexChildren[i].resize(*(file->mesh.getFirstChildrenLength() + i));

			uint * childLengths = file->mesh.getFirstChildrenLength();
			uint * children = file->mesh.getFirstChildren();
			for(uint i = 0; i < file->mesh.numVertexSplits; i++)
			{
				std::copy(children, children + *childLengths, vdlodStaticMesh.vertexChildren[i].begin());
				children += *childLengths;
				childLengths++;
			}

			// czytamy BV
			size_t bvSize;
 			BoundingVolumeType *bvType = file->mesh.getBoundingVolumeType();
			vdlodStaticMesh.boundingVolume = Habanero::MF::createBoundingVolume(bvType, &bvSize);

			if ((char*)file.get() + size != (char*)bvType + bvSize)
				RAISE(Exception, "Invalid file size");

			return vdlodStaticMesh;
		}
	}
}