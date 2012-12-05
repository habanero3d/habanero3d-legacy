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


#include "LightShader.h"

namespace Habanero
{
	LightShader::LightShader(const char *vertex, const char *fragment, const ShaderFlags &flags) : ShaderProgram(vertex, fragment, flags), flags(flags)
	{
		// TODO: brać pod uwagę flagi (doklejać nagłówek z shaderflags)
		GLuint shaderHandler = getProgramHandler();

		diffuseLocation = glGetUniformLocation(shaderHandler, "diffuse");
		normalMapLocation = glGetUniformLocation(shaderHandler, "normalMap");

		materialAmbientLocation = glGetUniformLocation(shaderHandler, "MaterialAmbient");
		materialSpecularLocation = glGetUniformLocation(shaderHandler, "MaterialSpecular");
		materialEmissiveLocation = glGetUniformLocation(shaderHandler, "MaterialEmissive");

		pointLighPosLocation = glGetUniformLocation(shaderHandler, "PointLightPos");
		pointLightColorLocation = glGetUniformLocation(shaderHandler, "PointLightColor");
		pointLightNumLocation = glGetUniformLocation(shaderHandler, "PointLightNum");
		pointLightRangeLocation = glGetUniformLocation(shaderHandler, "PointLightRange");

		directionalLighDirLocation = glGetUniformLocation(shaderHandler, "DirectionalLightDir");
		directionalLightColorLocation = glGetUniformLocation(shaderHandler, "DirectionalLightColor");
		directionalLightNumLocation = glGetUniformLocation(shaderHandler, "DirectionalLightNum");

		ambientLocation = glGetUniformLocation(shaderHandler, "Ambient");

		normalLocation = glGetAttribLocation(shaderHandler, "Normal");
	}

	void LightShader::setPointLights(const vector3f *positions, const Color *colors, const float *ranges)
	{
		//glUniform1i(pointLightNumLocation, num);
		glUniform3fv(pointLighPosLocation, 8, (float *)positions);
		glUniform4fv(pointLightColorLocation, 8, (float *)colors);
		glUniform1fv(pointLightRangeLocation, 8, ranges);
	}

	void LightShader::setDirectionalLights(const vector3f *directions, const Color *colors)
	{
		//glUniform1i(directionalLightNumLocation, num);
		glUniform3fv(directionalLighDirLocation, 8, (float *)directions);
		glUniform4fv(directionalLightColorLocation, 8, (float *)colors);
	}

	void LightShader::setMaterial(const Color& ambient, const Color& diffuse, const Color& specular)
	{
		RAISE(Exception, "Obsolete");
	}

	void LightShader::setAmbient(const Color &ambient)
	{
		glUniform4fv(ambientLocation, 1, (float*)&ambient);
	}

	void LightShader::setNormal(const vector3f &normal)
	{
		glVertexAttrib3fv(normalLocation, (float *)&normal);
	}

	void LightShader::setDiffuse(const GLuint textureId)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glUniform1i(diffuseLocation, 1);
	}

	void LightShader::setDiffuse(const Color &color)
	{
		glUniform4fv(diffuseLocation, 1, (float *)&color);
	}

	void LightShader::setNormalMap(GLuint textureId)
	{
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glUniform1i(normalMapLocation, 4);
	}
}
