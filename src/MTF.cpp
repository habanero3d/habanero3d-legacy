#include "MTF.h"
#include "File.h"
#include "Exception.h"
#include "UnifiedManager.h"

namespace Habanero
{
	namespace MTF2
	{
		Habanero::Material *loadFromFile(const char *fileName, std::map<uint, std::string> &textures)
		{
			uint64 size;
                        // kolejne Kuflowe obejœcie buga gcc
			std::unique_ptr<File, MappingDeleter> file((File*)Habanero::File::map(fileName, Habanero::File::Read, &size).release());
			if (file->signature != signature)
				RAISE(Exception, "Invalid MTF signature");
			
			ShaderFlags flags = file->material.flags;
			 
			Habanero::Material *material = new Habanero::Material();

			char * filePos = (char*)(&file->material.flags + 1);
			if(flags.getColorAmbient())
			{
				material->setAmbient(*((Color*)filePos));
				filePos += sizeof(Color);
			}
			else
			{
				Specific_ptr<Texture> tex = TextureManager::getInstance().getResource(textures[*((uint*)filePos)].c_str());
				material->setAmbient(tex);
				tex->load();
				filePos += sizeof(uint);
			}

			if(flags.getColorDiffuse())
			{
				material->setDiffuse(*((Color*)filePos));
				filePos += sizeof(Color);
			}
			else
			{
				Specific_ptr<Texture> tex = TextureManager::getInstance().getResource(textures[*((uint*)filePos)].c_str());
				material->setDiffuse(tex);
				tex->load();
				filePos += sizeof(uint);
			}

			if(flags.getColorSpecular())
			{
				material->setSpecular(*((Color*)filePos));
				filePos += sizeof(Color);
			}
			else
			{
				Specific_ptr<Texture> tex = TextureManager::getInstance().getResource(textures[*((uint*)filePos)].c_str());
				material->setSpecular(tex);
				tex->load();
				filePos += sizeof(uint);
			}

			if(flags.getColorEmissive())
			{
				material->setEmissive(*((Color*)filePos));
				filePos += sizeof(Color);
			}

			if(flags.getTransparency())
			{
				material->setTransparency(*((float*)filePos));
				filePos += sizeof(float);
			}
			else
			{
				Specific_ptr<Texture> tex = TextureManager::getInstance().getResource(textures[*((uint*)filePos)]);
				material->setTransparencyMap(tex);
				tex->load();
				filePos += sizeof(uint);
			}

			if (flags.getNormalMap())
            {
				Specific_ptr<Texture> tex = TextureManager::getInstance().getResource(textures[*((uint*)filePos)]);
				material->setNormalMap(tex);
				tex->load();
				filePos += sizeof(uint);
			}

			if (flags.getDisplacementMap())
            {
				Specific_ptr<Texture> tex = TextureManager::getInstance().getResource(textures[*((uint*)filePos)]);
				material->setDisplacementMap(tex);
				tex->load();
				filePos += sizeof(uint);
			}

			if (flags.getMysteryMap())
            {
				Specific_ptr<Texture> tex = TextureManager::getInstance().getResource(textures[*((uint*)filePos)]);
				material->setMysteryMap(tex);
				tex->load();
				filePos += sizeof(uint);
			}

			uint tmpDebug = filePos - (char*)&file->signature;
			if(filePos - (char*)&file->signature != size)
				RAISE(Exception, "Invalid file size");

			return material;
		}
	}
}