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


/*! @file
@author qfel, akuda
*/
#pragma once
#include "IResource.h"
#include "Mapping.h"

namespace Habanero
{
	class ResourceManager;

	class Texture : IResource
	{
		private:
			std::string filename;

		public:
			uint bits, width, height, size;
			void * data;
			uint oglId;

			Texture(const std::string& filename,
				Resource::Mapping &materials,
				Resource::Mapping &textures,
				Resource::Mapping &skeletons,
				Resource::Mapping &joints,
				Resource::Mapping &animations);

			~Texture();
			
			bool load();
			bool unload();
	};
}
