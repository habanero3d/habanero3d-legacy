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


#pragma once
#include "Color.h"
#include "Texture.h"
#include "ShaderFlags.h"
#include "Specific_ptr.h"

namespace Habanero
{	
	class Material
	{
	private:
		Material(const Material&);
		void operator =(const Material&);

		ShaderFlags flags;

		Color Ambient,
		      Diffuse,
		      Specular,
		      Emissive;
		float Transparency;

		Specific_ptr<Texture>	ambientTexture,
								diffuseTexture,
								specularTexture,
								normalMap,
								displacementMap,
								transparencyMap,
								mysteryMap;

	public:
		Material();

		ShaderFlags getFlags() const;

		void setAmbient(Color ambient);
		Color getAmbientColor() const;

		void setAmbient(const Specific_ptr<Texture> &ambient);
		Specific_ptr<Texture> getAmbientTexture() const;

		void setDiffuse(Color diffuse);
		Color getDiffuseColor() const;

		void setDiffuse(const Specific_ptr<Texture> &diffuse);
		Specific_ptr<Texture> getDiffuseTexture() const;

		void setSpecular(Color specular);
		Color getSpecularColor() const;

		void setSpecular(const Specific_ptr<Texture> &specular);
		Specific_ptr<Texture> getSpecularTexture() const;

		void setEmissive(Color emissive);
		Color getEmissive() const;

		void setTransparency(float transparency);
		float getTransparency() const;

		void setTransparencyMap(const Specific_ptr<Texture> &transparencyMap);
		Specific_ptr<Texture> getTransparencyMap() const;

		void setNormalMap(const Specific_ptr<Texture> &normalMap);
		Specific_ptr<Texture> getNormalMap() const;

		void setDisplacementMap(const Specific_ptr<Texture> &displacementMap);
		Specific_ptr<Texture> getDisplacementMap() const;

		void setMysteryMap(const Specific_ptr<Texture> &mysteryMap);
		Specific_ptr<Texture> getMysteryMap() const;
	};
}
