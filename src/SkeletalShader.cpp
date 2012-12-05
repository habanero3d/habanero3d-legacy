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


#include "SkeletalShader.h"

namespace Habanero
{
	SkeletalShader::SkeletalShader(const char *vertex, const char *fragment, const ShaderFlags &flags) : LightShader(vertex, fragment, flags)
	{
		GLuint shaderHandler = getProgramHandler();

		boneMatricesLocation = glGetUniformLocation(shaderHandler, "BoneMatrices");
		boneIndicesLocation = glGetAttribLocation(shaderHandler, "BoneIndices");
		boneWeightsLocation = glGetAttribLocation(shaderHandler, "BoneWeights");
	}

	void SkeletalShader::setMatrices(int numMatrices, const matrix4f *pose)
	{
		glUniformMatrix4fv(boneMatricesLocation, (int)numMatrices, false, (float *)pose);
	}

	void SkeletalShader::setBones(const uint *indices, const float *weights)
	{
		glVertexAttrib4fv(boneIndicesLocation, (float *)indices);
		glVertexAttrib4fv(boneWeightsLocation, weights);

	}

}
