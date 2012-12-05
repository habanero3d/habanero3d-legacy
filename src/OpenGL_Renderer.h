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
@author qfel
*/

#pragma once

#include <vector>

#include "Shader.h"
#include "vector3.h"
#include "Memory.h"
#include "GeometryTask.h"
#include "ProcessedScene.h"
#include "ShaderProgram.h"
#include "ShaderManager.h"
#include "Camera.h"

#include "OpenGL.h"

namespace Habanero
{
	class OpenGL_Renderer
	{
	private:
		OpenGL_Renderer(const OpenGL_Renderer&); //shielding
		void operator =(const OpenGL_Renderer&); //shielding

		ShaderManager *shaders;

		template <void (OpenGL_Renderer::*Handler)(const GeometryTask&)>
		void drawGeometryTask(const GeometryTask &task);

		void drawSkeletalAnimatedObject(const GeometryTask &task);
		void drawStaticObject(const GeometryTask &task);
		void drawLines(const GeometryTask &task);
		void drawPoints(const GeometryTask &task);
		void drawDebug(const GeometryTask &task);
		void drawDebugPoints(const GeometryTask &task);

		// Wyodrębnione, bo jest wykorzystywane w wołanych funkcjach
		// mega hack - przy każdej zmianie shadera wysyłamy znowu światła
		static const int MaxLightsNum = 16;
		float pointLightsRanges[MaxLightsNum];
		vector3f pointLightsPositions[MaxLightsNum];
		Color pointLightsColor[MaxLightsNum];

		vector3f directionalLightsDirections[MaxLightsNum];
		Color directionalLightsColor[MaxLightsNum];
		Color ambient;

		int trianglesRendered;

	public:
		OpenGL_Renderer();
		~OpenGL_Renderer();
		void drawScene(ProcessedScene &scene, const Camera &cam, int * trianglesNum = NULL);
	};
}