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


#include "Memory.h"
#include "OBB.h"
#include "HabMath.h"
#include "AABB.h"
#include "Sphere.h"
#include "plane.h"

namespace Habanero
{
	template <typename Real>
	bool OBB<Real>::growToContain(const vector3<Real> &point)
	{
		bool resized = false;
		for(int i = 0; i < 3; ++i)
		{
			vector3<Real> * Trans;//= ; // którys z tych 3
			if(i == 0) Trans = &ox;
			else if(i == 1) Trans = &oy;
			else Trans = &oz;

			vector3<Real> dir = Trans->normalized();

			plane<Real> s1(center + *Trans, dir),
						s2(center - *Trans, -dir);
			
			Real dist1 = s1.distance(point);
			Real dist2 = s2.distance(point);

			vector3<Real> dodatek;

			if(dist1 > dist2)
			{
				if(dist1 > 0)
				{
					dodatek = dir*dist1;
					*Trans += dodatek;
					resized = true;
				}
			}
			else
			{
				if(dist2 > 0)
				{
					dodatek = -dir*dist2;
					*Trans += dodatek;
					resized = true;
				}
			}
		}
		return resized;
	}

	template <typename Real>
	OBB<Real>::OBB()
	{
	}

	template <typename Real>
	OBB<Real>::OBB(const AABB<Real> &aabb) :
		center((aabb.xmin + aabb.xmax)/2, (aabb.ymin + aabb.ymax)/2, (aabb.zmin + aabb.zmax)/2),
		ox((aabb.xmax - aabb.xmin)/2, 0, 0),
		oy(0, (aabb.ymax - aabb.ymin)/2, 0),
		oz(0, 0, (aabb.zmax - aabb.zmin)/2)
	{
	}

	template <typename Real>
	OBB<Real>::OBB(const vector3<Real> &p, const vector3<Real> &x, const vector3<Real> &y, const vector3<Real> &z) :
		center(p),
		ox(x),
		oy(y),
		oz(z)
	{
	}

	template <typename Real>
	void OBB<Real>::calculatePoints(vector3<Real> *output) const
	{
		output[0] =	center - ox - oy + oz;  // <dolne>
		output[1] = center + ox - oy + oz;
		output[2] = center + ox - oy - oz;
		output[3] = center - ox - oy - oz;	// </dolne> @michauzo - wydaje mi sie, ze był błąd - poprawiłem pierwszy + na -
		output[4] = center - ox + oy + oz;  // <górne>
		output[5] = center + ox + oy + oz;
		output[6] = center + ox + oy - oz;
		output[7] = center - ox + oy - oz;	// </górne> @michauzo - i tu to samo
	}

#pragma region IBoundingVolume
	template <typename Real>
	PlaneInterferenceType OBB<Real>::planeInterference(const plane<Real> &p) const
	{
		vector3<Real> points[8];
		calculatePoints(points);
		bool wasOver = false;
		bool wasUnder = false;
		for(int i = 0; i < 8; i++)	// można spróbować optymalizować tego fora
			if(p.distance(points[i]) > 0)
				wasOver = true;
			else
				wasUnder = true;
		if(wasOver && wasUnder)
			return PlaneInterferenceType::Through;
		else if(wasOver)
			return PlaneInterferenceType::Over;
		else
			return PlaneInterferenceType::Under;
		return PlaneInterferenceType::Over;
	}

	template <typename Real>
	BoundingVolumeType OBB<Real>::getType() const
	{
		return BoundingVolumeType::OBB;
	}

	template <typename Real>
	void OBB<Real>::zero()
	{
		ox.x = ox.y = ox.z = oy.x = oy.y = oy.z = oz.x = oz.y = oz.z = 0;	// C-C-C-COMBO BREAKER!
	}

	template <typename Real>
	bool OBB<Real>::contains(const vector3<Real> &p) const
	{
		vector3<Real> dist = center - p;
		if(abs(dot(dist, ox)) <= ox.length() && abs(dot(dist, oy)) <= oy.length() && abs(dot(dist, oz)) <= oz.length())
			return true;
		return false;
	}

	template <typename Real>
	bool OBB<Real>::contains(const IBoundingVolume<Real> &bbox) const
	{
		switch(bbox.getType())
		{
			case BoundingVolumeType::AABB:
	            return contains(dynamic_cast<const AABB<Real>&>(bbox));
			case BoundingVolumeType::OBB:
	            return contains(dynamic_cast<const OBB<Real>&>(bbox));
			case BoundingVolumeType::Sphere:
	            return contains(dynamic_cast<const Sphere<Real>&>(bbox));
		}
		return false;
	}

	template <typename Real>
	bool OBB<Real>::collides(const IBoundingVolume<Real> &bbox) const
	{
	    switch(bbox.getType())
	    {
			case BoundingVolumeType::AABB:
	            return collides(dynamic_cast<const AABB<Real>&>(bbox));
	        case BoundingVolumeType::OBB:
	            return collides(dynamic_cast<const OBB<Real>&>(bbox));
	        case BoundingVolumeType::Sphere:
	            return collides(dynamic_cast<const Sphere<Real>&>(bbox));
	    }
		return false;
	}

	template <typename Real>
	InterferenceType OBB<Real>::interferes(const IBoundingVolume<Real> &bbox) const
	{
	    switch(bbox.getType())
	    {
	        case BoundingVolumeType::AABB:
	            return interferes(dynamic_cast<const AABB<Real>&>(bbox));
	        case BoundingVolumeType::OBB:
	            return interferes(dynamic_cast<const OBB<Real>&>(bbox));
	        case BoundingVolumeType::Sphere:
	            return interferes(dynamic_cast<const Sphere<Real>&>(bbox));
	    }
		return InterferenceType::NoInterference;
	}

	template <typename Real>
	bool OBB<Real>::growToContain(const IBoundingVolume<Real> &bbox)
	{
		switch(bbox.getType())
	    {
			case BoundingVolumeType::AABB:
	            return growToContain(dynamic_cast<const AABB<Real>&>(bbox));
	        case BoundingVolumeType::OBB:
	            return growToContain(dynamic_cast<const OBB<Real>&>(bbox));
	        case BoundingVolumeType::Sphere:
	            return growToContain(dynamic_cast<const Sphere<Real>&>(bbox));
			default: HASSERT(false);
	    }
	}

	template <typename Real>
	bool OBB<Real>::growToContain(const IBoundingVolume<Real> &bbox, const matrix4<Real> &transformation)
	{
		switch(bbox.getType())
	    {
			case BoundingVolumeType::AABB:
	            return growToContain(dynamic_cast<const AABB<Real>&>(bbox), transformation);
	        case BoundingVolumeType::OBB:
	            return growToContain(dynamic_cast<const OBB<Real>&>(bbox), transformation);
	        case BoundingVolumeType::Sphere:
	            return growToContain(dynamic_cast<const Sphere<Real>&>(bbox), transformation);
			default: HASSERT(false);
	    }
	}


	template <typename Real>
	std::unique_ptr<IBoundingVolume<Real>> OBB<Real>::clone() const
	{
		return unique(new OBB<Real>(*this));
	}
#pragma endregion
	
#pragma region AABB
	template <typename Real>
	bool OBB<Real>::contains(const AABB<Real> &aabb) const
	{
		vector3<Real> points[8];
		aabb.calculatePoints(points);
		for(int i = 0; i < 8; i++)
			if(!contains(points[i]))
				return false;
		return true;
	}

	template <typename Real>
	bool OBB<Real>::collides(const AABB<Real> &aabb) const
	{
		OBB<Real> that(aabb);
		return collides(that);
	}

	template <typename Real>
	InterferenceType OBB<Real>::interferes(const AABB<Real> &aabb) const
	{
		if(contains(aabb))
			return InterferenceType::Contains;
		if(collides(aabb))
			return InterferenceType::Collides;
		return InterferenceType::NoInterference;
	}

	template <typename Real>
	bool OBB<Real>::growToContain(const AABB<Real> &aabb)
	{
		vector3<Real> points[8];
		aabb.calculatePoints(points);
		bool resized = false;
		
		for(int i = 0; i < 8; i++)
			if(growToContain(points[i]))
				resized = true;

		return resized;
	}

	template <typename Real>
	bool OBB<Real>::growToContain(const AABB<Real> &aabb, const matrix4<Real> &transformation)
	{
		vector3<Real> points[8];
		aabb.calculatePoints(points);
		bool resized = false;
		
		for(int i = 0; i < 8; i++)
			if(growToContain(transformation.transformVertex(points[i])))
				resized = true;

		return resized;
	}
#pragma endregion

#pragma region OBB
	template <typename Real>
	bool OBB<Real>::contains(const OBB<Real> &obb) const
	{
		vector3<Real> points[8];
		obb.calculatePoints(points);
		for(int i = 0; i < 8; i++)
			if(!contains(points[i]))
				return false;
		return true;
	}


	template <typename Real>
	bool OBB<Real>::collides(const OBB<Real> &obb) const
	{
		// Separating planes theorem
		// Mocno inspirowane:
		// http://www.gamasutra.com/view/feature/3383/simple_intersection_tests_for_games.php?page=5

		// prosty test sferyczny - optymalizuje
		if ( (center - obb.center).length() >= (center - (ox + oy + oz)).length() + (obb.center - (obb.ox + obb.oy + obb.oz)).length() )
		    return false;

#pragma region Inicjalizacja strukturek
		// translacja, w układzie obiektu - TO NIE TO SAMO CO UKŁAD WYZNACZANY PRZEZ OBB!
		vector3<Real> v = obb.center - center; 
		// translacja w układzie wyznaczonym przez OBB (this)
		Real T[] = { dot(ox.normalized(), v), dot(oy.normalized(), v), dot(oy.normalized(), v) };
		// baza obb w układzie wyznaczonym przez OBB (this)
		Real R[3][3];
	    R[0][0] = dot(ox.normalized(), obb.ox.normalized());
		R[0][1] = dot(ox.normalized(), obb.oy.normalized());
		R[0][2] = dot(ox.normalized(), obb.oz.normalized());

		R[1][0] = dot(oy.normalized(), obb.ox.normalized());
		R[1][1] = dot(oy.normalized(), obb.oy.normalized());
		R[1][2] = dot(oy.normalized(), obb.oz.normalized());

		R[2][0] = dot(oz.normalized(), obb.ox.normalized());
		R[2][1] = dot(oz.normalized(), obb.oy.normalized());
		R[2][2] = dot(oz.normalized(), obb.oz.normalized());
#pragma endregion

        /* Sprawdzanie czy istnieje separating axis spośród 15 możliwych, jeśli nie
		   można znaleźć, tzn że się przecinają */
		Real ra, rb, t;
		// w bazie A
#pragma region Baza A
		ra = ox.length();
		rb = obb.ox.length() * abs(R[0][0])
			+ obb.oy.length() * abs(R[0][1])
			+ obb.oz.length() * abs(R[0][2]);
		t = abs(T[0]);

        if( t > ra + rb )
			return false;

		ra = oy.length();
		rb = obb.ox.length() * abs(R[1][0])
			+ obb.oy.length() * abs(R[1][1])
			+ obb.oz.length() * abs(R[1][2]);
		t = abs(T[1]);

        if( t > ra + rb )
	        return false;

		ra = oz.length();
		rb = obb.ox.length() * abs(R[2][0])
			+ obb.oy.length() * abs(R[2][1])
			+ obb.oz.length() * abs(R[2][2]);
		t = abs(T[2]);

		if( t > ra + rb )
			return false;
#pragma endregion

		// w bazie B
#pragma region Baza B
		ra = ox.length() * abs(R[0][0])
			+ oy.length() * abs(R[1][0])
			+ oz.length() * abs(R[2][0]);
		rb = obb.ox.length();
		t = abs(T[0] * R[0][0] + T[1] * R[1][0] + T[2] * R[2][0]);

		if( t > ra + rb )
                return false;

		ra = ox.length() * abs(R[0][1])
			+ oy.length() * abs(R[1][1])
			+ oz.length() * abs(R[2][1]);
		rb = obb.oy.length();
		t = abs(T[0] * R[0][1] + T[1] * R[1][1] + T[2] * R[2][1]);

		if( t > ra + rb )
                return false;

		ra = ox.length() * abs(R[0][2])
			+ oy.length() * abs(R[1][2])
			+ oz.length() * abs(R[2][2]);
		rb = obb.oz.length();
		t = abs(T[0] * R[0][2] + T[1] * R[1][2] + T[2] * R[2][2]);

		if( t > ra + rb )
			return false;
#pragma endregion

        // 9 crossów
#pragma region Produkty
        //L = A0 x B0
		ra = oy.length() * abs(R[2][0]) + oz.length() * abs(R[1][0]);
        rb = obb.oy.length() * abs(R[0][2]) + obb.oz.length() * abs(R[0][1]);
        t = abs(T[2]*R[1][0] - T[1]*R[2][0]);

		if( t > ra + rb )
			return false;

        //L = A0 x B1
		ra = oy.length() * abs(R[2][1]) + oz.length() * abs(R[1][1]);
        rb = obb.ox.length() * abs(R[0][2]) + obb.oz.length() * abs(R[0][0]);
        t = abs(T[2]*R[1][1] - T[1]*R[2][1]);

        if( t > ra + rb )
			return false;

        //L = A0 x B2
		ra = oy.length() * abs(R[2][2]) + oz.length() * abs(R[1][2]);
        rb = obb.ox.length() * abs(R[0][1]) + obb.oy.length() * abs(R[0][0]);
        t = abs(T[2]*R[1][2] - T[1]*R[2][2]);

        if( t > ra + rb )
	       return false;

        //L = A1 x B0
		ra = ox.length() * abs(R[2][0]) + oz.length() * abs(R[0][0]);
        rb = obb.oy.length() * abs(R[1][2]) + obb.oz.length() * abs(R[1][1]);
        t = abs(T[0]*R[2][0] - T[2]*R[0][0]);

        if( t > ra + rb )
			return false;

        //L = A1 x B1
		ra = ox.length() * abs(R[2][1]) + oz.length() * abs(R[0][1]);
        rb = obb.ox.length() * abs(R[1][2]) + obb.oz.length() * abs(R[1][0]);
        t = abs(T[0]*R[2][1] - T[2]*R[0][1]);

        if( t > ra + rb )
			return false;

        //L = A1 x B2
		ra = ox.length() * abs(R[2][2]) + oz.length() * abs(R[0][2]);
        rb = obb.ox.length() * abs(R[1][1]) + obb.oy.length() * abs(R[1][0]);
        t = abs(T[0]*R[2][2] - T[2]*R[0][2]);

        if( t > ra + rb )
			return false;

        //L = A2 x B0
		ra = ox.length() * abs(R[1][0]) + oy.length() * abs(R[0][0]);
        rb = obb.oy.length() * abs(R[2][2]) + obb.oz.length() * abs(R[2][1]);
        t = abs(T[1]*R[0][0] - T[0]*R[1][0]);

        if( t > ra + rb )
			return false;

        //L = A2 x B1
		ra =ox.length() * abs(R[1][1]) + oy.length() * abs(R[0][1]);
        rb = obb.ox.length() * abs(R[2][2]) + obb.oz.length() * abs(R[2][0]);
        t = abs(T[1]*R[0][1] - T[0]*R[1][1]);

        if( t > ra + rb )
            return false;

        //L = A2 x B2
		ra = ox.length() * abs(R[1][2]) + oy.length() *abs(R[0][2]);
        rb = obb.ox.length() * abs(R[2][1]) + obb.oy.length() * abs(R[2][0]);
        t = abs(T[1]*R[0][2] - T[0]*R[1][2]);

        if( t > ra + rb )
            return false;
#pragma endregion
        /* nie znaleziono separating axis, przecinają się */
		return true;
	}

	template <typename Real>
	InterferenceType OBB<Real>::interferes(const OBB<Real> &obb) const
	{
		if(contains(obb))
			return InterferenceType::Contains;
		if(collides(obb))
			return InterferenceType::Collides;
		return InterferenceType::NoInterference;
	}

	template <typename Real>
	bool OBB<Real>::growToContain(const OBB<Real> &obb)
	{
		vector3<Real> points[8];
		obb.calculatePoints(points);
		bool resized = false;
		
		for(int i = 0; i < 8; i++)
			if(growToContain(points[i]))
				resized = true;

		return resized;
	}

	template <typename Real>
	bool OBB<Real>::growToContain(const OBB<Real> &obb, const matrix4<Real> &transformation)
	{
		vector3<Real> points[8];
		obb.calculatePoints(points);
		bool resized = false;
		
		for(int i = 0; i < 8; i++)
			if(growToContain(transformation.transformVertex(points[i])))
				resized = true;

		return resized;
	}
#pragma endregion

#pragma region Sphere
	template <typename Real>
	bool OBB<Real>::contains(const Sphere<Real> &sph) const
	{
		vector3<Real> points[] = {
			vector3<Real>(sph.center + sph.radius * ox.normalized()),
			vector3<Real>(sph.center - sph.radius * ox.normalized()),
			vector3<Real>(sph.center + sph.radius * oy.normalized()),
			vector3<Real>(sph.center - sph.radius * oy.normalized()),
			vector3<Real>(sph.center + sph.radius * oz.normalized()),
			vector3<Real>(sph.center - sph.radius * oz.normalized())
		};

		if(contains(sph.center))
		{
			for(int i = 0; i < 6; i++)
				if(!contains(points[i]))
					return false;
			return true;
		}
		return false;
	}

	template <typename Real>
	bool OBB<Real>::collides(const Sphere<Real> &sph) const
	{
		// przerzutujmy sobie sferę na układ OBB:
		vector3<Real> sphCenter(
			dot(sph.center, ox) / ox.length(),
			dot(sph.center, oy) / oy.length(),
			dot(sph.center, oz) / oz.length());

		// wyznaczam "p_min", "p_max" dla OBB w jego układzie
		Real xmin, ymin, zmin, xmax, ymax, zmax;
		xmin = center.x - ox.length();
		ymin = center.y - oy.length();
		zmin = center.z - oz.length();

		xmax = center.x + ox.length();
		ymax = center.y + oy.length();
		zmax = center.z + oz.length();

		// algorytm Avro:
		Real s,d = 0;
		if ( sphCenter.x < xmin )
		{
		    s = sphCenter.x - xmin;
		    d += s * s;
		}
		else if ( sphCenter.x > xmax )
		{
		    s = sphCenter.x - xmax;
		    d += s * s;
		}
		
		if ( sphCenter.y < ymin )
		{
		    s = sphCenter.y - ymin;
		    d += s * s;
		}		
		else if ( sphCenter.y > ymax )
		{
		    s = sphCenter.y - ymax;
		    d += s * s;
		}
		
		if ( sphCenter.z < zmin )
		{
		    s = sphCenter.z - zmin;
		    d += s * s;
		}		
		else if ( sphCenter.z > zmax )
		{
		    s = sphCenter.z - zmax;
		    d += s * s;
		}
		    
		return d <= sph.radius * sph.radius;
	}

	template <typename Real>
	InterferenceType OBB<Real>::interferes(const Sphere<Real> &sph) const
	{
		if(contains(sph))
			return InterferenceType::Contains;
		if(collides(sph))
			return InterferenceType::Collides;
		return InterferenceType::NoInterference;
	}

	template <typename Real>
	bool OBB<Real>::growToContain(const Sphere<Real> &sph)
	{

		// TODO
		vector3<Real> points[] = {
			vector3<Real>(sph.center + sph.radius * ox.normalized()),
			vector3<Real>(sph.center - sph.radius * ox.normalized()),
			vector3<Real>(sph.center + sph.radius * oy.normalized()),
			vector3<Real>(sph.center - sph.radius * oy.normalized()),
			vector3<Real>(sph.center + sph.radius * oz.normalized()),
			vector3<Real>(sph.center - sph.radius * oz.normalized())
		};	// te punkty muszą się zawierać w obb
		return false;
	}

	template <typename Real>
	bool OBB<Real>::growToContain(const Sphere<Real> &sph, const matrix4<Real> &transformation)
	{

		// TODO
		vector3<Real> points[] = {
			vector3<Real>(sph.center + sph.radius * ox.normalized()),
			vector3<Real>(sph.center - sph.radius * ox.normalized()),
			vector3<Real>(sph.center + sph.radius * oy.normalized()),
			vector3<Real>(sph.center - sph.radius * oy.normalized()),
			vector3<Real>(sph.center + sph.radius * oz.normalized()),
			vector3<Real>(sph.center - sph.radius * oz.normalized())
		};	// te punkty muszą się zawierać w obb
		return false;
	}
#pragma endregion

	template class OBB<float>;
	template class OBB<double>;
};
