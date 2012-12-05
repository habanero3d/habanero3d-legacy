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


#include "plane.h"

namespace Habanero
{
	template <typename Real>
	plane<Real>::plane()
	{
	}
	
		
	template <typename Real>
	plane<Real>::plane(Real x, Real y, Real z, Real w) : 
		normal(x, y, z)
	{
		Real len = normal.length();
		normal /= len;
		d = w / len;
	}

	template <typename Real>
	plane<Real>::plane(const vector3<Real> &n, Real w) :
		normal(n)
	{
		Real len = normal.length();
		normal /= len;
		d = w / len;
	}

	template <typename Real>
	plane<Real>::plane(const vector3<Real> &p, const vector3<Real> &n) :
		normal(n.normalized())
	{
		d = - dot(p, normal);
	}

	template <typename Real>
	plane<Real>::plane(const vector3<Real> &p1, const vector3<Real> &p2, const vector3<Real> &p3)
	{
		normal = cross((p2 - p1), (p3 - p2));
		normal.normalize();
		d = - dot(p1, normal);
	}

	template <typename Real>
	Real plane<Real>::distance(const vector3<Real> &p) const
	{
		return (dot(normal, p) + d);
	}

	template <typename Real>
	void plane<Real>::flip()
	{
		normal *= -1;
		d = -d;
	}
	
	template class plane<float>;
	template class plane<double>;

};