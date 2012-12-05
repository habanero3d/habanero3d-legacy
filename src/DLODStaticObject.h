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

/*! @file
@author bialy
*/

#pragma once

#include "DLODStaticMesh.h"
#include "Spatial.h"

namespace Habanero
{
	class DLODStaticObject : public Spatial
	{
	private:
		Specific_ptr<DLODStaticMesh> mesh;

		float computeLODLevel(const Frustum<float>&) const;

		bool isLODForced;
		uint forcedLevel;

	protected:
		void fillScene(const Frustum<float>&, ProcessedScene &scene) const;
		void initBoundingVolume();
	public:
		DLODStaticObject(Specific_ptr<DLODStaticMesh>);
		const IBoundingVolume<float>* getPickableBoundingVolume() const;

		bool getIsLODForced() const;
		void setIsLODForced(bool value);

		uint getForcedLevel() const;
		void setForcedLevel(uint value);
	};
}