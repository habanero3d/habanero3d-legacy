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
Author: Biały
modifications by akuda
 */

#pragma once

#include <string>
#include <map>

#include "GenericManager.h"
#include "Specific_ptr.h"

#include "StaticMesh.h"
#include "SkinnedMesh.h"
#include "CLODStaticMesh.h"
#include "CLODSkinnedMesh.h"
#include "DLODStaticMesh.h"
#include "DLODSkinnedMesh.h"
#include "VDLODStaticMesh.h"
#include "VDLODSkinnedMesh.h"
#include "Skeleton.h"
#include "Texture.h"
#include "HeightMapMesh.h"

namespace Habanero
{
	template <class ResourceType>
	class UnifiedManager : public GenericManager<ResourceType>
	{
		typedef std::map<std::string, ulong> nameToIdType;
	private:
		nameToIdType nameToId;

		const char * resourceName;

	public:		
		UnifiedManager(const char * resourceName) : GenericManager<ResourceType>(this), resourceName(resourceName)
		{
			GenericManager<ResourceType>::setGlobalManagerInstance(this);
			// TODO: powinien wskazywać na jakiś zasób błędu
			GenericManager<ResourceType>::setGlobalErrorPointer(new Specific_ptr<ResourceType>(NULL));
		}

		Specific_ptr<ResourceType> discoverFile(const char *filename,
			const char *name)
		{
			Resource::Mapping m = Resource::Mapping();
			Resource::Mapping t = Resource::Mapping();
			Resource::Mapping s = Resource::Mapping();
			Resource::Mapping j = Resource::Mapping();
			Resource::Mapping a = Resource::Mapping();
			return discoverFile(filename, name, m, t, s, j, a);
		}

		Specific_ptr<ResourceType> discoverFile(const char *filename,
			const char *name,
			Resource::Mapping &materials)
		{
			Resource::Mapping t = Resource::Mapping();
			Resource::Mapping s = Resource::Mapping();
			Resource::Mapping j = Resource::Mapping();
			Resource::Mapping a = Resource::Mapping();
			return discoverFile(filename, name, materials, t, s, j, a);
		}

		Specific_ptr<ResourceType> discoverFile(const char *filename,
			const char *name,
			Resource::Mapping &materials,
			Resource::Mapping &textures)
		{
			Resource::Mapping s = Resource::Mapping();
			Resource::Mapping j = Resource::Mapping();
			Resource::Mapping a = Resource::Mapping();
			return discoverFile(filename, name, materials, textures, s, j, a);
		}

		Specific_ptr<ResourceType> discoverFile(const char *filename,
			const char *name,
			Resource::Mapping &materials,
			Resource::Mapping &textures,
			Resource::Mapping &skeletons)
		{
			Resource::Mapping j = Resource::Mapping();
			Resource::Mapping a = Resource::Mapping();
			return discoverFile(filename, name, materials, textures, skeletons, j, a);
		}

		Specific_ptr<ResourceType> discoverFile(const char *filename,
			const char *name,
			Resource::Mapping &materials,
			Resource::Mapping &textures,
			Resource::Mapping &skeletons,
			Resource::Mapping &joints)
		{
			Resource::Mapping a = Resource::Mapping();
			return discoverFile(filename, name, materials, textures, skeletons, joints, a);
		}
		
		Specific_ptr<ResourceType> discoverFile(const char *filename,
			const char *name,
			Resource::Mapping &materials,
			Resource::Mapping &textures,
			Resource::Mapping &skeletons,
			Resource::Mapping &joints,
			Resource::Mapping &animations)
		{
			std::string strName(name);
			nameToIdType::iterator it = nameToId.find(strName);

			// TODO: wywalić w ramach optymalizacji
			if (it != nameToId.end())
			{
				NEWLOG("%s Manager: Resource %s is already discovered.", resourceName, filename);
				return this->getPtrFromId(it->second);
			}

			ResourceType * resourcePointer = new ResourceType(std::string(filename), materials, textures, skeletons, joints, animations);
		
			Specific_ptr<ResourceType> ptr(resourcePointer);
			nameToId.insert(std::make_pair(strName, ptr.getId()));

			return ptr;
		}
		
		Specific_ptr<ResourceType> getResource(const std::string &name)
		{
			nameToIdType::iterator it = nameToId.find(std::string(name));
			if (it == nameToId.end())
			{
				NEWLOG("%s Manager: Error: Resource \"%s\" is not discovered.", resourceName, name.c_str());
				return GenericManager<ResourceType>::getErrorPointer();
			}
			return GenericManager<ResourceType>::getPtrFromId(it->second);
		}

		static UnifiedManager<ResourceType> & getInstance()
		{
			return (UnifiedManager<ResourceType> &) GenericManager<ResourceType>::getInstance();
		}

		static UnifiedManager<ResourceType> * getInstancePtr()
		{
			return (UnifiedManager<ResourceType> *) GenericManager<ResourceType>::getInstancePtr();
		}

		std::vector<std::string> getResourceNames()
		{
			std::vector<std::string> ret;
			for (auto it = nameToId.begin(); it != nameToId.end(); ++it)
				ret.push_back(it->first);
			return ret;
		}
	};

	typedef UnifiedManager<StaticMesh> StaticMeshManager;
	typedef UnifiedManager<SkinnedMesh> SkinnedMeshManager;
	typedef UnifiedManager<DLODStaticMesh> DLODStaticMeshManager;
	typedef UnifiedManager<DLODSkinnedMesh> DLODSkinnedMeshManager;
	typedef UnifiedManager<CLODStaticMesh> CLODStaticMeshManager;
	typedef UnifiedManager<CLODSkinnedMesh> CLODSkinnedMeshManager;
	typedef UnifiedManager<VDLODStaticMesh> VDLODStaticMeshManager;
	typedef UnifiedManager<VDLODSkinnedMesh> VDLODSkinnedMeshManager;
	typedef UnifiedManager<HeightMapMesh> HeightMapMeshManager;

	typedef UnifiedManager<Texture> TextureManager;
	typedef UnifiedManager<Skeleton> SkeletonManager;
}
