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
#include "CLODSMF.h"
#include "CLODSkinnedMesh.h"
#include "IBoundingVolume.h"

namespace Habanero
{
	uint CLODSkinnedMesh::getNumVertexSplits() const
	{
		return numVertexSplits;
	}

	GenericGeometry<SkinVertex> * CLODSkinnedMesh::moveForward(uint howMuch, std::vector<uint>& counters)
	{
		for(uint i = lastVertexSplits; i < lastVertexSplits + howMuch; i++)
		{
			const VSplit& split = vertexSplits[i];
			ExtendableSubMesh * sm = (ExtendableSubMesh *)(tmpGeometry->getSubMesh(split.subMeshId));

			for(int j = 0; j < split.numAdditions; j++)
			{
				MF2::TriangleAddition& addition = split.getTriangleAdditions()[j];
				for(int k = 0; k < 3; k++)
				{
					ExtendableSubMesh * sm = (ExtendableSubMesh *)(tmpGeometry->getSubMesh(split.subMeshId));
					sm->setIndex(counters[split.subMeshId] + k, addition.indices[k]);
				}
				counters[split.subMeshId] +=3;
			}

			for(int j = 0; j < split.numMoves; j++)
			{
				MF2::VertexMove& move = split.getVertexMoves()[j];

				bool performed = false;
				for(int ind = 0; ind < sm->getNumIndices(); ind += 3)
				{
					if(move.pattern[0] != sm->getIndex(ind)
					|| move.pattern[1] != sm->getIndex(ind + 1)
					|| move.pattern[2] != sm->getIndex(ind + 2))
						continue;	// these are not the triangles you are looking for, move along (pattern mismatch)

					for(int offs = 0; offs < 3; offs++)
						if(move.fromId == sm->getIndex(ind + offs))
						{
							sm->setIndex(ind + offs, move.toId);
							performed = true;
						}

					break;	// pattern was matched, it can be performed only once (there will be no exact triangles in mesh), no need to check more
				}

				HASSERT(performed);
			}

			sm->setNumIndices(counters[split.subMeshId]);
		}

		lastVertexSplits += howMuch;
		isCached = true;

		return tmpGeometry;
	}

	GenericGeometry<SkinVertex> * CLODSkinnedMesh::moveBackward(uint howMuch, std::vector<uint>& counters)
	{
		for(int i = lastVertexSplits - 1; i >= (int)lastVertexSplits - (int)howMuch; i--)
		{
			const VSplit& split = vertexSplits[i];
			ExtendableSubMesh * sm = (ExtendableSubMesh *)(tmpGeometry->getSubMesh(split.subMeshId));

			for(int j = 0; j < split.numMoves; j++)
			{
				MF2::VertexMove& move = split.getVertexMoves()[j];
				uint pattern[3] = {
					move.fromId == move.pattern[0] ? move.toId : move.pattern[0],
					move.fromId == move.pattern[1] ? move.toId : move.pattern[1],
					move.fromId == move.pattern[2] ? move.toId : move.pattern[2]
				};

				bool performed = false;
				for(int ind = 0; ind < sm->getNumIndices(); ind += 3)
				{
					if(pattern[0] != sm->getIndex(ind)
					|| pattern[1] != sm->getIndex(ind + 1)
					|| pattern[2] != sm->getIndex(ind + 2))
						continue;

					for(int offs = 0; offs < 3; offs++)
						if(move.toId == sm->getIndex(ind + offs))
						{
							sm->setIndex(ind + offs, move.fromId);
							performed = true;
						}

					break;
				}

				HASSERT(performed);
			}

			counters[split.subMeshId] -= 3 * split.numAdditions;
			sm->setNumIndices(counters[split.subMeshId]);
		}

		lastVertexSplits -= howMuch;
		isCached = true;

		return tmpGeometry;
	}

	GenericGeometry<SkinVertex> * CLODSkinnedMesh::getGeometry(uint vsplits)
	{
		HASSERT(vsplits >= 0 && vsplits <= numVertexSplits);

		if(isCached && vsplits == lastVertexSplits)
			return tmpGeometry;
		else if(isCached)
		{
			int difference = vsplits - lastVertexSplits;	// how much vsplits has to be perfomed (can be negative, in that case we have to reverse the operations)

			std::vector<uint> counters(geometry->getNumSubMeshes());
			for(int i = 0; i < geometry->getNumSubMeshes(); i++)
			{
				const ExtendableSubMesh * sm = (ExtendableSubMesh *)(tmpGeometry->getSubMesh(i));
				counters[i] = sm->getNumIndices();
			}

			if(difference > 0)
				return moveForward(difference, counters);
			else
				return moveBackward(-difference, counters);
		}

		lastVertexSplits = 0;
		std::vector<uint> counters(geometry->getNumSubMeshes());
		for(int i = 0; i < geometry->getNumSubMeshes(); i++)
		{
			const SubMesh * sm = geometry->getSubMesh(i);
			ExtendableSubMesh * extSubMesh = (ExtendableSubMesh *)(tmpGeometry->getSubMesh(i));
			counters[i] = sm->getNumIndices();
			// overwrite previous changes
			std::copy(sm->getIndices(), sm->getIndices() + sm->getNumIndices(), extSubMesh->getIndices());
		}
		return moveForward(vsplits, counters);;
	}

	CLODSkinnedMesh::CLODSkinnedMesh(const std::string &filename,
		Resource::Mapping &materials,
		Resource::Mapping &textures,
		Resource::Mapping &skeletons,
		Resource::Mapping &joints,
		Resource::Mapping &animations)
		: filename(filename), materials(materials), textures(textures), skeletons(skeletons)
	{
		setLoadState(Discovered);
	}

	CLODSkinnedMesh::~CLODSkinnedMesh()
	{
		unload();
	}

	bool CLODSkinnedMesh::load()
	{
		if(getLoadState() != Discovered)
			return false;

		InitInfo initInfo = CLODSMF::loadFromFile(filename.c_str(), materials, textures, skeletons);
		skeleton = initInfo.skeleton;
		boundingVolume = initInfo.boundingVolume;

		geometry = new GenericGeometry<SkinVertex>(initInfo.geometry, true);
		tmpGeometry = new GenericGeometry<SkinVertex>(initInfo.geometry);

		isCached = false;
		lastVertexSplits = 0;

		numVertexSplits = initInfo.vertexSplits.size();
		vertexSplits = uninitializedAlloc<VSplit>(initInfo.vertexSplits.size());
		for(int i = 0; i < initInfo.vertexSplits.size(); i++)
			new(&vertexSplits[i]) VSplit(initInfo.vertexSplits[i]);

		setLoadState(Loaded);
		return true;
	}

	bool CLODSkinnedMesh::unload()
	{
		if(getLoadState() == Discovered)
			return false;

		delete geometry;
		delete tmpGeometry;

		uninitializedDelete(vertexSplits, numVertexSplits);

		delete boundingVolume;
		setLoadState(Discovered);
		return true;
	}
}
