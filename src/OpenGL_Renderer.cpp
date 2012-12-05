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
@author akuda, qfel
*/
/*
TODO: Kosc nie ma rodzica -> noParent - to moze wywalic MS3D
*/
#include <algorithm>
#include <utility>
#include <memory>
#include <vector>

#include "OpenGL.h"
#include "SkeletalAnimatedObject.h"
#include "OpenGL_Renderer.h"
#include "StaticObject.h"
#include "LightShader.h"
#include "SkinVertex.h"
#include "Spatial.h"
#include "SubMesh.h"
#include "matrix4.h"
#include "vector3.h"
#include "Memory.h"
#include "HabMath.h"
#include "File.h"
#include "Shader.h"

//#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#pragma comment(lib, "glu32.lib")
#pragma warning(3:4365)

namespace Habanero
{
	OpenGL_Renderer::OpenGL_Renderer()
	{
		glewInit();
		//shaders = ShaderManager::getInstancePtr() != NULL ? ShaderManager::getInstancePtr() : new ShaderManager();
		shaders = new ShaderManager();
	}

	OpenGL_Renderer::~OpenGL_Renderer()
	{
		//delete ShaderManager::getInstancePtr();
		delete shaders;
	}

	void OpenGL_Renderer::drawScene(ProcessedScene &scene, const Camera &cam, int * trianglesNum)
	{
		trianglesRendered = 0;

        if (cam.wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT, GL_FILL);

		int allLightsNum = (int)scene.lights.size();
		HASSERT(allLightsNum <= MaxLightsNum);

		for (uint i = 0; i < MaxLightsNum; ++i)
		{
			pointLightsRanges[i] = 0.0;
			pointLightsPositions[i] = vector3f::zero;
			pointLightsColor[i] = Color(0,0,0,0);

			directionalLightsDirections[i] = vector3f::zero;
			directionalLightsColor[i] = Color(0,0,0,0);
		}

		std::sort(scene.lights.begin(), scene.lights.end(), [](const LightTask &l, const LightTask &r) {return l.Type < r.Type; });

		int i;
		for(i = 0; i < allLightsNum && scene.lights[i].Type == LightType::PointLight; i++)
		{
			pointLightsPositions[i] = cam.getViewMatrix().transformVertex(scene.lights[i].Data.position);
			pointLightsColor[i] = scene.lights[i].color;
			pointLightsRanges[i] = cam.getViewMatrix().transformNormal(vector3f(scene.lights[i].Data.radius, 0, 0)).length();
		}

		for(int j = i; i < allLightsNum && scene.lights[i].Type == LightType::DirectionalLight; i++)
		{
			directionalLightsDirections[i - j] = cam.getViewMatrix().transformNormal(scene.lights[i].Data.direction).normalized();
			directionalLightsColor[i - j] = scene.lights[i].color;
		}

		glEnable(GL_CULL_FACE);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(180 / M_PI * cam.getFOV(), cam.getRatio(), cam.getNearDist(), cam.getFarDist());

		//to ma wylecieć
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // tmp
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		glMatrixMode(GL_MODELVIEW);
		matrix4f tmpViewMatrix = cam.getViewMatrix();
		glLoadMatrixf((float*)&tmpViewMatrix);

		//std::sort(scene.geometry.begin(), scene.geometry.end(), [](const GeometryTask &l, const GeometryTask &r) { return l.shaderType < r.shaderType; });
        std::sort(scene.geometry.begin(), scene.geometry.end(), [=](const GeometryTask &l, const GeometryTask &r)->bool{return l.shaderType < r.shaderType; });
		std::vector<GeometryTask>::iterator it = scene.geometry.begin();

		ambient = scene.ambient;
		// Static
		// Jak to nie działa pod gcc to niedobrze, zgłosić się do Kufla!! Na dywanik!!
		for (; it != scene.geometry.end() && it->shaderType == ShaderType::ShaderNone;it++)
			drawGeometryTask<&OpenGL_Renderer::drawStaticObject>(*it);

		// Szkieletowa

		for (; it != scene.geometry.end() && it->shaderType == ShaderType::SkeletalAnimation; it++)
			drawGeometryTask<&OpenGL_Renderer::drawSkeletalAnimatedObject>(*it);

		// Debugi
		glDisable(GL_TEXTURE_2D);

		shaders->getNoneShaderProgram()->enable();
		glColor3f(1.0f, 0.0f, 0.0f);
		for (; it != scene.geometry.end() && it->shaderType == ShaderType::Lines; it++)
			drawGeometryTask<&OpenGL_Renderer::drawLines>(*it);
		glColor3f(0.0f, 1.0f, 0.0f);
		for (; it != scene.geometry.end() && it->shaderType == ShaderType::Points; it++)
			drawGeometryTask<&OpenGL_Renderer::drawPoints>(*it);
		shaders->getNoneShaderProgram()->disable();

		if(trianglesNum)
			*trianglesNum = trianglesRendered;
	}

	template <void (OpenGL_Renderer::*Handler)(const GeometryTask&)>
	void OpenGL_Renderer::drawGeometryTask(const GeometryTask &task)
	{
		glPushMatrix();
		glMultMatrixf((float*)&task.transformation);
		(this->*Handler)(task);
		glPopMatrix();
	}

#pragma region Drawing

	void OpenGL_Renderer::drawSkeletalAnimatedObject(const GeometryTask &task)
	{
		HASSERT(task.geometry->getVertexDataSize() % sizeof(SkinVertex) == 0);

		SkinVertex *vertices = (SkinVertex*)task.geometry->getVertexData();
		uint numVertices = task.geometry->getVertexDataSize() / sizeof(SkinVertex);
		matrix4<float> *pose = (matrix4<float>*)task.shaderArgs->getData();
		uint numMatrices = task.shaderArgs->getSize() / sizeof(matrix4f);
		HASSERT(numMatrices <= 64);
		
		SkeletalShader *shader = NULL;

		for (uint i = 0; i < task.geometry->getNumSubMeshes(); i++)
		{
			const SubMesh * subMesh = task.geometry->getSubMesh(i);
			
			//float color[4] = {1.,1.,1.,1.};
			/*Color red = Color(0.5, 0.5, 0);
			lightShader->setMaterial(red, red, red);*/

			shader = shaders->getSkeletalShaderProgram(subMesh->getMaterial()->getFlags());
			shader->enable();
			shader->setPointLights(pointLightsPositions, pointLightsColor, pointLightsRanges);
			shader->setDirectionalLights(directionalLightsDirections, directionalLightsColor);
			shader->setAmbient(ambient);

			//niestety na razie wsunięte do pętli, tak jak wysyłanie świateł...
			shader->setMatrices(numMatrices, pose);

			if(!!subMesh->getMaterial()) // TODO
			{
				// TODO:
				//lightShader->setMaterial(subMesh.material->Ambient, subMesh.material->Diffuse, subMesh.material->Specular);

				bool textured = false;

				glEnable(GL_TEXTURE_2D);

				if (subMesh->getMaterial()->getFlags().getTextureDiffuse())// && !!subMesh.material->getAmbientTexture())
				{													 // zakomentowane, bo co najwyżej będzie tekstura błędu...					
					shader->setDiffuse(subMesh->getMaterial()->getDiffuseTexture()->oglId);
					textured = true;
				}
				else
					shader->setDiffuse(subMesh->getMaterial()->getDiffuseColor());

				if(subMesh->getMaterial()->getFlags().getNormalMap())
				{
					shader->setNormalMap(subMesh->getMaterial()->getNormalMap()->oglId);
					textured = true;
				}
				
				if(!textured)
					glDisable(GL_TEXTURE_2D);
			
			}
			glBegin(GL_TRIANGLES);
			uint numIndices = subMesh->getNumIndices();
			for (uint j = 0; j < numIndices; j++)
			{
				shader->setBones(vertices[subMesh->getIndex(j)].jointIndices, vertices[subMesh->getIndex(j)].jointWeights);
				shader->setNormal(vertices[subMesh->getIndex(j)].normal);
				glTexCoord2fv((float*)&vertices[subMesh->getIndex(j)].texCoord);	// wysłany TexCoord
				glVertex3fv((float*)&vertices[subMesh->getIndex(j)].position);	// wysłany vertex
			}
			glEnd();

			trianglesRendered += numIndices / 3;

			shader->disable();
		}
	}

	void OpenGL_Renderer::drawStaticObject(const GeometryTask &task)
	{
		StaticVertex *vertices = (StaticVertex*)task.geometry->getVertexData();
		uint numVertices = task.geometry->getVertexDataSize() / sizeof(StaticVertex);
		
		for (uint i = 0; i < task.geometry->getNumSubMeshes(); i++)
		{
			const SubMesh * subMesh = task.geometry->getSubMesh(i);
			LightShader *shader = NULL;

			//float color[4] = {1.,1.,1.,1.};

			/*Color red = Color(0.5, 0.5, 0);
			lightShader->setMaterial(red, red, red);*/

			glDisable(GL_TEXTURE_2D);

			if(!!subMesh->getMaterial()) // TODO
			{
				// TODO:
				//lightShader->setMaterial(subMesh.material->Ambient, subMesh.material->Diffuse, subMesh.material->Specular);
				shader = shaders->getLightShaderProgram(subMesh->getMaterial()->getFlags());
				shader->enable();
				shader->setPointLights(pointLightsPositions, pointLightsColor, pointLightsRanges);
				shader->setDirectionalLights(directionalLightsDirections, directionalLightsColor);
				shader->setAmbient(ambient);
				bool textured = false;

				glEnable(GL_TEXTURE_2D);

				if (subMesh->getMaterial()->getFlags().getTextureDiffuse())// && !!subMesh.material->getAmbientTexture())
				{													 // zakomentowane, bo co najwyżej będzie tekstura błędu...					
					shader->setDiffuse(subMesh->getMaterial()->getDiffuseTexture()->oglId);
					textured = true;
				}
				else
					shader->setDiffuse(subMesh->getMaterial()->getDiffuseColor());

				if(subMesh->getMaterial()->getFlags().getNormalMap())
				{
					shader->setNormalMap(subMesh->getMaterial()->getNormalMap()->oglId);
					textured = true;
				}
				
				if(!textured)
					glDisable(GL_TEXTURE_2D);
			}

			glBegin(GL_TRIANGLES);
			uint numIndices = subMesh->getNumIndices();
			for (uint j = 0; j < numIndices; j++)
			{
				if(shader)
					shader->setNormal(vertices[subMesh->getIndex(j)].normal);
				glTexCoord2fv((float*)&vertices[subMesh->getIndex(j)].texCoord);
				glVertex3fv((float*)&vertices[subMesh->getIndex(j)].position);
			}
			glEnd();

			trianglesRendered += numIndices / 3;

			shader->disable();
		}
	}

	void OpenGL_Renderer::drawLines(const GeometryTask &task)
	{
		typedef std::pair<vector3f, vector3f> Line;
		ShaderArgs<Line> *args = static_cast<ShaderArgs<Line> *>(task.shaderArgs.get());
		const Line *lines = args->getData();
		glBegin(GL_LINES);
		for (uint i = args->getCount(); i--; )
		{
			glVertex3fv((float*)&lines[i].first);
			glVertex3fv((float*)&lines[i].second);
		}
		glEnd();
	}

	void OpenGL_Renderer::drawPoints(const GeometryTask &task)
	{
		ShaderArgs<vector3f> *args = static_cast<ShaderArgs<vector3f> *>(task.shaderArgs.get());
		const vector3f *points = args->getData();
		glBegin(GL_POINTS);
		for (uint i = args->getCount(); i--; )
			glVertex3fv((float*)&points[i]);
		glEnd();
	}
#pragma endregion

}
