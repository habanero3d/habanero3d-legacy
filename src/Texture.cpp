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


/*!
@file
@author qfel, akuda
@brief Niezależna od systemu część implementacji obiektu tekstury.
 */
#include "OpenGL.h"
#include "Texture.h"

#include <SFML/Graphics/Image.hpp>
#include <string.h>

namespace Habanero
{
	Texture::Texture(const std::string& filename,
			Resource::Mapping &materials,
			Resource::Mapping &textures,
			Resource::Mapping &skeletons,
			Resource::Mapping &joints,
			Resource::Mapping &animations) : IResource(), filename(filename)
	{
		setLoadState(LoadStateType::Discovered);
		width = 0;
		height = 0;
		bits = 0;
		data = NULL;
	}
	
	
	bool Texture::load()
	{
		if (getLoadState() == LoadStateType::Loaded)
			return true;

		sf::Image image;
		bool loadingSuccessful;
		
		loadingSuccessful = image.loadFromFile(filename);

		if(!loadingSuccessful)
			return false;
		
		width = image.getSize().x;
		height = image.getSize().y;

		size = width * height;

		//TODO: przepisać to na coś ładnego.
		data = (void *) new uint[size];

		//tu nie da się dodać sprawdzenia poprawności, bo memcpy nie zwraca takiego info
		memcpy(data, image.getPixelsPtr(), 4 * size);

		//TODO: TO MUSI STĄD WYLECIEĆ
		glGenTextures(1, &oglId);
		glBindTexture(GL_TEXTURE_2D, oglId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,GL_RGBA, GL_UNSIGNED_BYTE, data);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		setLoadState(LoadStateType::Loaded);
		return true;
	}
	
	bool Texture::unload()
	{
		if(getLoadState() == LoadStateType::Loaded)
		{
			glDeleteTextures(1, &oglId);
			delete[] (byte*) data;
			setLoadState(Discovered);
		}
		return true; //Dodane, żeby nie było błędu...
	}


	Texture::~Texture()
	{
		if(getLoadState() != LoadStateType::Discovered)
		{
			unload();
		}
	}

}
