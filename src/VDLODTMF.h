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
@brief View-Dependant LOD sTatic Mesh Format
@author bialy
*/

#pragma once

#include "Mapping.h"
#include "VDLODStaticMesh.h"

namespace Habanero
{
	class StaticVertex;
	class BoundingVolumeType;

	struct SubMesh;

	namespace MF2
	{
		class SubMesh;
		struct VSplit;
		struct VertexMove;
		struct TriangleAddition;
		struct VSplitRefinement;
	}

	namespace VDLODTMF
	{
		const uint signature = '1MTV';
		struct VDLODStaticMesh
		{
			uint numVertices;
			uint numSubMeshes;
			uint numVertexSplits;

			StaticVertex* getVertices();
			Habanero::MF2::SubMesh * getFirstSubMesh();
			Habanero::uint * getFirstMaxIndex();	/* dafuq did i just write?.. intellisense siê przypieprza jak nie ma Habanero::uint */
			Habanero::MF2::VSplit* getFirstVSplit();
			
			Habanero::MF2::VSplitRefinement * getFirstRefinement();

			uint * getFirstChildrenLength();
			uint * getFirstChildren();
			
			BoundingVolumeType* getBoundingVolumeType();
		};

		struct File
		{
			uint signature;
			VDLODStaticMesh mesh;
		};

		Habanero::VDLODStaticMesh::InitInfo loadFromFile(const char * fileName, Resource::Mapping &materials, Resource::Mapping &textures);
	}

}