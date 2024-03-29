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


#pragma once

#include "Polyhedron.h"
#include "vector3.h"
#include "plane.h"

#include <vector>

namespace Habanero
{	
	template <typename Real>
	class Frustum : public Polyhedron<Real>
	{
	public:
		vector3<Real> viewPoint;

		Frustum(const std::vector<plane<Real >> &_planes,
				const std::vector<vector3<Real >> &_vertices,
				const std::vector<struct Polyhedron<Real>::PolyhedronEdge>& _edges,
				const vector3<Real>& _viewPoint);
		
		//TODO usunąć to jak skończy Biały
		const plane<Real>& front() const { return this->planes[0]; }
		const plane<Real>& rear() const { return this->planes[1]; }
		const plane<Real>& bottom() const { return this->planes[2]; }
		const plane<Real>& top() const { return this->planes[3]; }
		const plane<Real>& left() const { return this->planes[4]; }
		const plane<Real>& right() const { return this->planes[5]; }
		
		void transform(const matrix4<Real>& matrix);

	};
}
