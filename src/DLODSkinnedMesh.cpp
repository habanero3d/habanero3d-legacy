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

#include "Memory.h"
#include "DLODSMF.h"
#include "DLODSkinnedMesh.h"
#include "HabMath.h"
#include "IBoundingVolume.h"


namespace Habanero
{
	DLODSkinnedMesh::DLODSkinnedMesh(const std::string &filename,
			Resource::Mapping &materials,
			Resource::Mapping &textures,
			Resource::Mapping &skeletons,
			Resource::Mapping &joints,
			Resource::Mapping &animations)
		: filename(filename), materials(materials), textures(textures), skeletons(skeletons)
	{
	}

	DLODSkinnedMesh::~DLODSkinnedMesh()
	{
		//delete boundingVolume;
		unload();
	}

	uint DLODSkinnedMesh::getNumLODLevels() const
	{
		return numLODLevels;
	}

	GenericGeometry<SkinVertex> * DLODSkinnedMesh::getGeometry(float lodLevel) const
	{
		int c = ceil(lodLevel);
		return lodLevels[c].getGeometry();
	}

	bool DLODSkinnedMesh::load()
	{
		if (getLoadState() != Discovered)
			return false;
		InitInfo initInfo = DLODSMF::loadFromFile(filename.c_str(), materials, textures, skeletons);
		boundingVolume = initInfo.boundingVolume;
		skeleton = initInfo.skeleton;
		numLODLevels = initInfo.dlodLevels.size();
		lodLevels = uninitializedAlloc<DLODLevel<SkinVertex>>(numLODLevels);
		for(uint i = 0; i < numLODLevels; i++)
			new(&lodLevels[i]) DLODLevel<SkinVertex>(initInfo.dlodLevels[i]);
		setLoadState(Loaded);
		return true;
	}

	bool DLODSkinnedMesh::unload()
	{
		if(getLoadState() == Discovered)
			return false;
		uninitializedDelete(lodLevels, numLODLevels);
		delete boundingVolume;
		setLoadState(Discovered);
		return true;
	}
}