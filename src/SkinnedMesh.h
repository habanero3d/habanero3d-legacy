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

#pragma once
#include <vector>

#include "GenericGeometry.h"
#include "SkinVertex.h"
#include "Skeleton.h"
#include "SubMesh.h"
#include "Mapping.h"
#include "IResource.h"

namespace Habanero
{
	template <typename Real> class IBoundingVolume;

	class SkinnedMesh : IResource
	{
	public:
		struct InitInfo
		{
			IBoundingVolume<float> *boundingVolume; // TODO: Brzyydkie (wskaznik zmienia wlasciciela)
			Specific_ptr<Skeleton>skeleton;
			GenericGeometry<SkinVertex>::InitInfo geometry;
		};

	private:
		SkinnedMesh(const SkinnedMesh&);
		void operator =(const SkinnedMesh&);

		/*!
		@param Vertices Tablica wierzchołków, kopiowana na użytek obiektu.
		@param SubMeshes Tablica submesh'y, kopiowana na użytek obiektu.
		W praktyce dałoby się uniknąć narzutu na tworzenie tymczasowej tablicy tylko po to, by przekazać
		ją do skopiowania jako argument, ale w ten sposób abstrachujemy od sposobu w jaki zostanie zaalokowana.
		*/

		GenericGeometry<SkinVertex> *geometry;
		uint numVertices;
		std::string filename;
		Resource::Mapping materials, textures, skeletons, joints, animations;

	public:
		//TODO: Prywatyzacja i gettery
		IBoundingVolume<float> *boundingVolume;
		Specific_ptr<Skeleton> skeleton;

		SkinnedMesh(const std::string &filename, 
					Resource::Mapping &materials, 
					Resource::Mapping &textures, 
					Resource::Mapping &skeletons, 
					Resource::Mapping &joints, 
					Resource::Mapping &animations);
		~SkinnedMesh();

		GenericGeometry<SkinVertex> *getGeometry() const;
		uint getNumVertices() const;
		uint getNumSubMeshes() const;
		const SkinVertex& getVertex(uint index) const;
		const SubMesh * getSubMesh(uint index) const;

		bool load();
		bool unload();
	};
}
