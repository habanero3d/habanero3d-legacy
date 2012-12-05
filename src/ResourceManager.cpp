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

#include "ResourceManager.h"
#include "File.h"
#include <string.h>
#include <fstream>
#include <sstream>

namespace Habanero
{
	ResourceManager * ResourceManager::singleInstance = NULL;
	
	ResourceManager * ResourceManager::getInstance()
	{
		return singleInstance;
	}
	
	ResourceManager::ResourceManager()
	{
		HASSERT(singleInstance == NULL);
		singleInstance = this;

		textureManagerPtr = new TextureManager("Texture");
		skeletonManagerPtr = new SkeletonManager("Skeleton");
		staticMeshManagerPtr = new StaticMeshManager("StaticMesh");
		skinnedMeshManagerPtr = new SkinnedMeshManager("SkinnedMesh");
		heightMapMeshManagerPtr = new HeightMapMeshManager("HeightMap");

		dlodStaticMeshManagerPtr = new DLODStaticMeshManager("DLODStaticMesh");
		clodStaticMeshManagerPtr = new CLODStaticMeshManager("CLODStaticMesh");
		dlodSkinnedMeshManagerPtr = new DLODSkinnedMeshManager("DLODSkinnedMesh");
		clodSkinnedMeshManagerPtr = new CLODSkinnedMeshManager("CLODSkinnedMesh");
		vdlodStaticMeshManagerPtr = new VDLODStaticMeshManager("VDLODStaticMesh");
		vdlodSkinnedMeshManagerPtr = new VDLODSkinnedMeshManager("VDLODSkinnedMesh");
	}
	
	ResourceManager::~ResourceManager()
	{
		// uwaga, ważna kolejność!
		delete vdlodSkinnedMeshManagerPtr;
		delete vdlodStaticMeshManagerPtr;
		delete dlodSkinnedMeshManagerPtr;
		delete skinnedMeshManagerPtr;		
		delete clodStaticMeshManagerPtr;
		delete dlodStaticMeshManagerPtr;
		delete clodSkinnedMeshManagerPtr;
		delete staticMeshManagerPtr;
		delete heightMapMeshManagerPtr;

		delete textureManagerPtr;
		delete skeletonManagerPtr;
		singleInstance = NULL;
	}
	
	void ResourceManager::discoveryDirectory(std::string directory)
	{
		if (! isDirectory(directory + "/meshes"))
		{
			NEWLOG("ResourceManager: Content directory does not contains meshes directory.");
			return;
		}
		texturesDiscovery(directory + "/textures", "");
		if (! isDirectory(directory + "/textures"))
		{
			NEWLOG("ResourceManager: Content directory does not contains textures directory.");
			return;
		}
		meshesDiscovery(directory + "/meshes", "");
	}

	void ResourceManager::texturesDiscovery(std::string fullPath, std::string prefix)
	{
		std::vector<std::string> files;
		if (! listDirectory(fullPath, files))
		{
			NEWLOG("ResourceManager: cannot list directory \"%s\".", fullPath.c_str());
			return;
		}
		for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
		{
			std::string path = fullPath + "/" + *it;
			if (isDirectory(path))
				texturesDiscovery(path, (prefix == "" ? *it : prefix + "." + *it));
			else
				textureManagerPtr->discoverFile(path.c_str(), path.c_str());
		}
	}

	void ResourceManager::meshesDiscovery(std::string fullPath, std::string prefix)
	{
		std::vector<std::string> files;
		if (! listDirectory(fullPath, files))
		{
			NEWLOG("ResourceManager: cannot list directory \"%s\".", fullPath.c_str());
			return;
		}
		bool onlyFiles = true, onlyDirectories = true;

		for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
		{
			std::string path = fullPath + "/" + *it;
			if (isDirectory(path))
				onlyFiles = false;
			else
				onlyDirectories = false;
		}
		if (!onlyFiles && !onlyDirectories)
		{
			NEWLOG("ResourceManager: Ignoring directory \"%s\": contains files and directories.", fullPath.c_str());
			return;
		}
		if (onlyFiles)
		{
			if (prefix == "")
				NEWLOG("ResourceManager: Error: root meshes directory contains files only.");
			loadMesh(fullPath, prefix, files);
		}
		else
		{
			for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
			{
				std::string path = fullPath + "/" + *it;
				meshesDiscovery(path, (prefix == "" ? *it : prefix + "." + *it));
			}
		}
	}

	void ResourceManager::loadMesh(std::string path, std::string name, std::vector<std::string> files)
	{
		//TODO: dopisać tę metodę
		Resource::Mapping materials, textures, skeleton, joints, animations;
		bool i2nFound = false,
			saf = false, smf = false, tmf = false,
			dlodtmf = false, dlodsmf = false,
			clodtmf = false, clodsmf = false,
			vdlodtmf = false, vdlodsmf = false,
			hmf = false;
		for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
		{
			if (!strcmp(it->c_str(), "i2n"))
			{
				std::string fullPath = path + "/" + *it;
				i2nFound = true;
				parseI2N(fullPath, path, materials, textures, skeleton, joints, animations);
			}
		}
		if (! i2nFound)
		{
			NEWLOG("ResourceManager: Error! i2n not found in \"%s\"", path.c_str());
			return;
		}
		for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
		{
			std::string fullPath = path + "/" + *it;
			const char *s = fullPath.c_str();
			int len = fullPath.size();
			if (!strcmp(it->c_str(), "i2n") || !strcmp(s + len - 4, ".mtf"))
				continue;
			else if (!strcmp(s + len - 4, ".bmp"))
				textureManagerPtr->discoverFile(s, s);
			else if (!strcmp(s + len - 4, ".saf"))
			{
				if (saf)
				{
					NEWLOG("ResourceManager: Warning! Multiple .saf files in %s", path.c_str());
					continue;
				}
				skeletonManagerPtr->discoverFile(s, s);
				saf = true;
			}
			else if (!strcmp(s + len - 4, ".smf"))
			{
				if (smf)
				{
					NEWLOG("ResourceManager: Warning! Multiple .smf files in %s", path.c_str());
					continue;
				}
				skinnedMeshManagerPtr->discoverFile(s, name.c_str(), materials, textures, skeleton, joints, animations);
				smf = true;
			}
			else if (!strcmp(s + len - 4, ".tmf"))
			{
				if (tmf)
				{
					NEWLOG("ResourceManager: Warning! Multiple .tmf files in %s", path.c_str());
					continue;
				}
				staticMeshManagerPtr->discoverFile(s, name.c_str(), materials, textures);
				tmf = true;
			}
			else if (!strcmp(s + len - 8, ".dlodtmf"))
			{
				if (dlodtmf)
				{
					NEWLOG("ResourceManager: Warning! Multiple .dlodtmf files in %s", path.c_str());
					continue;
				}
				dlodStaticMeshManagerPtr->discoverFile(s, name.c_str(), materials, textures);
				dlodtmf = true;
			}
			else if (!strcmp(s + len - 8, ".clodtmf"))
			{
				if (clodtmf)
				{
					NEWLOG("ResourceManager: Warning! Multiple .clodtmf files in %s", path.c_str());
					continue;
				}
				clodStaticMeshManagerPtr->discoverFile(s, name.c_str(), materials, textures);
				clodtmf = true;
			}
			else if (!strcmp(s + len - 8, ".dlodsmf"))
			{
				if(dlodsmf)
				{
					NEWLOG("ResourceManager: Warning! Multiple .dlodsmf files in %s", path.c_str());
					continue;
				}
				dlodSkinnedMeshManagerPtr->discoverFile(s, name.c_str(), materials, textures, skeleton);
				dlodsmf = true;
			}
			else if (!strcmp(s + len - 8, ".clodsmf"))
			{
				if(clodsmf)
				{
					NEWLOG("ResourceManager: Warning! Multiple .clodsmf files in %s", path.c_str());
					continue;
				}
				clodSkinnedMeshManagerPtr->discoverFile(s, name.c_str(), materials, textures, skeleton);
				clodsmf = true;
			}
			else if (!strcmp(s + len - 9, ".vdlodtmf"))
			{
				if(vdlodtmf)
				{
					NEWLOG("ResourceManager: Warning! Multiple .vdlodtmf files in %s", path.c_str());
					continue;
				}
				vdlodStaticMeshManagerPtr->discoverFile(s, name.c_str(), materials, textures, skeleton);
				vdlodtmf = true;
			}
			else if (!strcmp(s + len - 9, ".vdlodsmf"))
			{
				if(vdlodsmf)
				{
					NEWLOG("ResourceManager: Warning! Multiple .vdlodsmf files in %s", path.c_str());
					continue;
				}
				vdlodSkinnedMeshManagerPtr->discoverFile(s, name.c_str(), materials, textures, skeleton);
				vdlodsmf = true;
			}
			else if (!strcmp(s + len - 4, ".hmf"))
			{
				if(hmf)
				{
					NEWLOG("ResourceManager: Warning! Multiple .hmf files in %s", path.c_str());
					continue;
				}
				heightMapMeshManagerPtr->discoverFile(s, name.c_str(), materials, textures);
				hmf = true;
			}
			else
				NEWLOG("ResourceManager: Ignoring %s", s);
		}
		NEWLOG("ResourceManager: Object %s discovered.\n", name.c_str());	
	}

	void ResourceManager::getI2NBlock(std::ifstream &file, 
										 Resource::Mapping &mapping, 
										 bool files, 
										 const std::string &dir, 
										 const std::string &ext)
	{
		uint id;
		const int bufferSize = 80;
		char buffer[bufferSize];
		while (!file.eof() && file.peek() != '#')
		{
			file >> id;
			if (!(file.peek() == '.'))
				return;
			
			file.ignore(2, ' ');
			
			file.getline(buffer, bufferSize);
			fixLineEnd(buffer);
			
			if (files)
			{
				std::string path = dir + '/' + std::string(buffer) + ext;
				Normalize(path);
				mapping[id] = path;
			}
			else
			{
				mapping[id] = std::string(buffer);
			}
		}
	}

	bool ResourceManager::parseI2N(const std::string &path, 
									  const std::string &dir, 
									  Resource::Mapping &materials, 
									  Resource::Mapping &textures, 
									  Resource::Mapping &skeleton, 
									  Resource::Mapping &joints, 
									  Resource::Mapping &animations)
	{
		std::ifstream file(path);
		if (!file.is_open())
			return false;
		const int bufferSize = 80;
		char buffer[bufferSize];
		while (!file.eof() && file.peek() == '#')
		{
			file.getline(buffer, bufferSize);
			fixLineEnd(buffer);
			
			if (!strncmp(buffer, "#materials",10))
				getI2NBlock(file, materials, true, dir, ".mtf");
			if (!strncmp(buffer, "#textures",9))
				getI2NBlock(file, textures, true, dir);
			if (!strncmp(buffer, "#skeleton",9))
				getI2NBlock(file, skeleton, true, dir, ".saf");
			if (!strncmp(buffer, "#joints",7))
				getI2NBlock(file, joints, false);
			if (!strncmp(buffer, "#animations",11))
				getI2NBlock(file, animations, false);
		}
		file.close();
		return true;
	}

	/*
	 * Not supper effective way to remove \r and \n from the end of line
	 */
	inline void ResourceManager::fixLineEnd(char * line)
	{
		int finalCharacterPos = strlen(line) - 1;
		while(finalCharacterPos >= 0)
		{
			if(	line[finalCharacterPos] == '\r'	||
				line[finalCharacterPos] == '\n'	)
			{
				line[finalCharacterPos] = '\0';
				finalCharacterPos--;
			} else break;
		}
	}

}
