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

#include "MFCommon.h"

namespace Habanero
{
	class VSplit
	{		
	private:
		MF2::VertexMove * vertexMoves;
		MF2::TriangleAddition * triangleAdditions;
	public:
		struct InitInfo
		{
			uint subMeshId;
			std::vector<MF2::VertexMove> moves;
			std::vector<MF2::TriangleAddition> additions;
		};

		const uint subMeshId;
		const uint numMoves;
		const uint numAdditions;

		VSplit(const VSplit::InitInfo& initInfo);
		~VSplit();

		uint getNumVertexMoves() const;
		MF2::VertexMove * getVertexMoves() const;

		uint getNumTriangleAdditions() const;
		MF2::TriangleAddition * getTriangleAdditions() const;
	};
}
