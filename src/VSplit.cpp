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

#include "VSplit.h"

#include <hash_map>

namespace Habanero
{
	VSplit::VSplit(const VSplit::InitInfo& initInfo) : subMeshId(initInfo.subMeshId),
		numAdditions(initInfo.additions.size()),
		numMoves(initInfo.moves.size())
	{
		vertexMoves = new MF2::VertexMove[numMoves];
		std::copy(initInfo.moves.begin(), initInfo.moves.end(), vertexMoves);

		triangleAdditions = new MF2::TriangleAddition[numAdditions];
		std::copy(initInfo.additions.begin(), initInfo.additions.end(), triangleAdditions);
	}

	VSplit::~VSplit()
	{
		delete [] vertexMoves;
		delete [] triangleAdditions;
	}

	uint VSplit::getNumVertexMoves() const
	{
		return numMoves;
	}

	MF2::VertexMove * VSplit::getVertexMoves() const
	{
		return vertexMoves;
	}

	uint VSplit::getNumTriangleAdditions() const
	{
		return numAdditions;
	}

	MF2::TriangleAddition * VSplit::getTriangleAdditions() const
	{
		return triangleAdditions;
	}
};