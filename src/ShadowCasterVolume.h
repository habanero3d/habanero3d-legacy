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

/*author: akuda */

/*
 Implementuje bryłę zawierającą w całości cień i Frustę. Idea jest tutaj:
 http://www.gamedev.net/page/resources/_/technical/game-programming/shadow-caster-volumes-for-the-culling-of-potential-shadow-casters-r2330
  
 acz implementujemy to prościej.
 */

#pragma once

#include "Polyhedron.h"
#include "Frustum.h"

namespace Habanero {
	
	template <typename Real>
	class ShadowCasterVolume : public Polyhedron<Real>
	{
	};
}