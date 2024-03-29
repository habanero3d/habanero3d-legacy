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

/*
 This file is for testing purposes only, the engine does not rely on it.
 */

/*! @file
@author bialy
*/



#pragma once

#include <string>
#include <fstream>

#include "Spatial.h"

namespace Habanero
{
	class FPSLogger : public Spatial
	{
	private:
		std::ofstream fileStream;
		const float maxTime;
		float recordedTime;
		int recordedFrames;
	public:
		FPSLogger(const std::string& filename, float range, float time = 1000);

		virtual void updateSelf(float time, bool parentUpdated);
	};
}