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

#include "ShaderProgram.h"
#include "ShaderFlags.h"
#include "vector3.h"
#include "Color.h"

namespace Habanero
{
	class LightShader : public ShaderProgram
	{
	private:
		const ShaderFlags flags;

		GLint ambientLocation;
		GLint diffuseLocation;

		GLint normalMapLocation;

		GLint materialAmbientLocation;
		GLint materialSpecularLocation;
		GLint materialEmissiveLocation;

		GLint pointLighPosLocation;
		GLint pointLightColorLocation;
		GLint pointLightNumLocation;
		GLint pointLightRangeLocation;

		GLint directionalLighDirLocation;
		GLint directionalLightColorLocation;
		GLint directionalLightNumLocation;

		GLint normalLocation;
	public:
		LightShader(const char *vertex, const char *fragment, const ShaderFlags &flags);

		void setPointLights(const vector3f *positions, const Color *colors, const float *ranges);
		void setDirectionalLights(const vector3f *directions, const Color *colors);
		void setMaterial(const Color& ambient, const Color& diffuse, const Color& specular);
		void setAmbient(const Color &ambient);
		void setNormal(const vector3f &normal);

		void setDiffuse(GLuint textureId);
		void setDiffuse(const Color &color);

		void setNormalMap(GLuint textureId);
	};
}