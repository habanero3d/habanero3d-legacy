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
Author: Andrzej "akuda" Skalski
 */

#pragma once

#include <vector>
#include "Mapping.h"
#include "UnifiedManager.h"

namespace Habanero
{
	class ResourceManager
	{
		friend class RootObject;
	private:
		static ResourceManager * singleInstance;

		TextureManager * textureManagerPtr;
		StaticMeshManager * staticMeshManagerPtr;
		SkeletonManager * skeletonManagerPtr;
		SkinnedMeshManager * skinnedMeshManagerPtr;
		HeightMapMeshManager * heightMapMeshManagerPtr;
		CLODStaticMeshManager * clodStaticMeshManagerPtr;
		CLODSkinnedMeshManager * clodSkinnedMeshManagerPtr;
		DLODStaticMeshManager * dlodStaticMeshManagerPtr;
		DLODSkinnedMeshManager * dlodSkinnedMeshManagerPtr;
		VDLODStaticMeshManager * vdlodStaticMeshManagerPtr;
		VDLODSkinnedMeshManager * vdlodSkinnedMeshManagerPtr;

		std::map<std::string, std::string> availableResources;
		std::vector<std::string> allFiles;
		
	public:
		static ResourceManager * getInstance();
		
		/*
		 * Funkcja ładująca zasoby ze wsakzanego katalogu. W trakcie implementacji
		 */
		void discoveryDirectory(std::string directory);
		void loadMesh(std::string fullPath, std::string name, std::vector<std::string> files);

	private:
		ResourceManager();
		~ResourceManager();
		void meshesDiscovery(std::string fullPath, std::string prefix);
		void texturesDiscovery(std::string fullPath, std::string prefix);
		bool parseI2N(const std::string &path, 
					  const std::string &dir, 
					  Resource::Mapping &materials, 
					  Resource::Mapping &textures, 
					  Resource::Mapping &skeleton, 
					  Resource::Mapping &joints, 
					  Resource::Mapping &animations);
		void getI2NBlock(std::ifstream &file, 
						 Resource::Mapping &mapping, 
						 bool files = false, 
						 const std::string &dir = "", 
						 const std::string &ext = "");
                
                inline void fixLineEnd(char * line);
	};
}
