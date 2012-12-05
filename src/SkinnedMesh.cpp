/*
Copyright 2009-2012 Andrzej Skalski, Piotr Kufel, Piotr Białecki, Michał Żochowski, and Michał Szczepaniak
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
@author: akuda, qfel
*/
#include <algorithm>

#include "Memory.h"
#include "SubMesh.h"
#include "SkinVertex.h"
#include "SkinnedMesh.h"
#include "IBoundingVolume.h"
#include "SMF.h"

namespace Habanero
{

	SkinnedMesh::SkinnedMesh(const std::string &filename,
			Resource::Mapping &materials,
			Resource::Mapping &textures,
			Resource::Mapping &skeletons,
			Resource::Mapping &joints,
			Resource::Mapping &animations) :
		IResource(),	
		filename(filename), 
		materials(materials), 
		textures(textures), 
		skeletons(skeletons), 
		joints(joints), 
		animations(animations), 
		boundingVolume(NULL)
	{
		numVertices = 0;
		geometry = NULL;
		setLoadState(Discovered);
	}

	SkinnedMesh::~SkinnedMesh()
	{
		//delete boundingVolume;		// FIX? czemu tu był brany adres wskaźnika?
		unload();
	}

	GenericGeometry<SkinVertex> *SkinnedMesh::getGeometry() const
	{
		return geometry;
	}

	const SkinVertex& SkinnedMesh::getVertex(uint index) const
	{
		return geometry->getVertex(index);
	}

	const SubMesh * SkinnedMesh::getSubMesh(uint index) const
	{
		return geometry->getSubMesh(index);
	}

	uint SkinnedMesh::getNumVertices() const
	{
		return numVertices;
	}

	uint SkinnedMesh::getNumSubMeshes() const
	{
		return geometry->getNumSubMeshes();
	}

	bool SkinnedMesh::load()
	{
		if (getLoadState() != Discovered)
			return false;
		skeleton.~Specific_ptr();
		InitInfo initInfo = SMF2::loadFromFile(filename.c_str(), materials, textures, skeletons, joints, animations);
		skeleton = initInfo.skeleton;
		boundingVolume = initInfo.boundingVolume;
		geometry = new GenericGeometry<SkinVertex>(initInfo.geometry);
		numVertices = initInfo.geometry.vertices.size();
		setLoadState(Loaded);
		return true;
	}
	
	bool SkinnedMesh::unload()
	{
		if(getLoadState() == Discovered)
			return false;
		delete geometry;
		delete boundingVolume;
		setLoadState(Discovered);
		return true;
	}
}