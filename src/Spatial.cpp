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

#include "Spatial.h"

#include <vector>

#include "Memory.h"
#include "Sphere.h"

#include "IBoundingVolume.h"
#include "ProcessedScene.h"

#include "Camera.h"

#include "HabMath.h"
#include "AABB.h"
#include "Sphere.h"
#include "line3.h"

namespace Habanero
{
	Spatial::Spatial(std::unique_ptr<IBoundingVolume<float>> &&boundingVolume,
					 bool isLightSource) :
		parentToLocal(matrix4f::identity),
		parentToLocalR(matrix4f::identity),
		dirty(false), dirtyR(false), growBV(false), rebuildBV(false), parentChanged(false),
		boundingVolume(std::move(boundingVolume)),
		visualizeBoundingVolume(false),
		_isLightSource(isLightSource),
		_isShadowCaster(false)
	{
	}

	const IBoundingVolume<float>& Spatial::getBoundingVolume() const
	{
		return *boundingVolume;
	}

	const IBoundingVolume<float>* Spatial::getPickableBoundingVolume() const
	{
		return NULL;
	}

	inline bool Spatial::isLightSource() const
	{
		return _isLightSource;
	}
		
	inline bool Spatial::isShadowCaster() const
	{
		return _isShadowCaster;
	}
	
	const matrix4f& Spatial::getParentToLocal() const
	{
		HASSERT(!dirty);
		return parentToLocal;
	}

	const matrix4f& Spatial::getParentToLocalR() const
	{
		HASSERT(!dirtyR);
		return parentToLocalR;
	}

	const matrix4f& Spatial::getWorldToLocal() const
	{
		HASSERT(!dirty);
		return worldToLocal;
	}

	const matrix4f& Spatial::getWorldToLocalR() const
	{
		HASSERT(!dirtyR);
		return worldToLocalR;
	}

	void Spatial::setParentToLocal(const matrix4f &m)
	{
		dirty = false;
		dirtyR = true;

		if(getParent())
			getParent()->rebuildBV = true;
		parentToLocal = m;
	}

	void Spatial::setParentToLocalR(const matrix4f &m)
	{
		dirty = true;
		dirtyR = false;

		if(getParent())
			getParent()->rebuildBV = true;
		parentToLocalR = m;
	}

	void Spatial::attach(Spatial *what)
	{
		HASSERT(what);
		HASSERT(!what->parent);
		what->parentChanged = true;
		Node<Spatial>::attach(what);
	}

	void Spatial::detach()
	{
		parentChanged = true;
		if (parent != NULL)
			getParent()->rebuildBV = true;
		Node<Spatial>::detach();
	}

	/*
	 * browBV to bool który mówi MOJEMU parent, że MI URÓSŁ BV, i trzeba
	 * wywołać parentowi growToContain na wszelki wypadek.
	 * rebuildBV mówi, że BV mi się zmniejszył i trzeba robić rebuilda
	 */
	void Spatial::updateTreeInternal(float time, bool updateTransform)
	{
		HASSERT(!(dirty && dirtyR));
		
		bool dirtyEither = dirty || dirtyR;
		if (dirty)
		{
			dirty = false;
			parentToLocal =	parentToLocalR.inversed();
		}
		else if (dirtyR)
		{
			dirtyR = false;
			parentToLocalR = parentToLocal.inversed();
		}
		if (dirtyEither || parentChanged || updateTransform)
		{
			updateTransform = true;

			Spatial *parent = getParent();
			if (parent)
			{
				worldToLocal = parent->worldToLocal * parentToLocal;
				worldToLocalR = parentToLocalR * parent->worldToLocalR;
			}
			else
			{
				worldToLocal = parentToLocal;
				worldToLocalR = parentToLocalR;
			}
		}

		updateSelf(time, updateTransform);
		
		
		for (Spatial *nd = getFirstChild(); nd; nd = nd->getNextSibling())
		{
			nd->updateTreeInternal(time, updateTransform);
			rebuildBV = rebuildBV || nd->rebuildBV;
			nd->rebuildBV = false;
			growBV = growBV || nd->growBV;
		}
		if (rebuildBV)
			rebuildBoundingVolume();
		else if (growBV)
		{
			growBV = false;
			growBoundingVolume();
		}
		growBV = growBV || dirtyEither || parentChanged;
		parentChanged = false;
	}

	void Spatial::update(float time)
	{
		updateTreeInternal(time, false);
		growBV = false;
		rebuildBV = false;
	}

	/*
	TODO: to tylko funkcja pomocnicza na czas pisania renderera. trzeba
	ją wywalić!
	*/
	std::vector<const Spatial*> Spatial::getLights(std::vector<const Spatial*> &lights) const
	{
		if(this->isLightSource())
			lights.push_back(this);
		
		for (const Spatial *nd = getFirstChild(); nd; nd = nd->getNextSibling())
			nd->getLights(lights);
		return lights;
	}
	
	void Spatial::rebuildBoundingVolume()
	{
		boundingVolume->zero();
		initBoundingVolume();
		for (Spatial *nd = getFirstChild(); nd; nd = nd->getNextSibling())
		{
			nd->growBV = false;
			boundingVolume->growToContain(nd->getBoundingVolume(), nd->getParentToLocalR());
		}
	}

	void Spatial::growBoundingVolume()
	{
		for (Spatial *nd = getFirstChild(); nd; nd = nd->getNextSibling())
			if (nd->growBV)
			{
				nd->growBV = false;
				if (boundingVolume->growToContain(nd->getBoundingVolume(), nd->getParentToLocalR()))
					growBV = true;
			}
	}

	void Spatial::recursiveFillScene(const Frustum<float> &fru, ProcessedScene &scene) const
	{	
		Frustum<float> nfru = fru;
		nfru.transform(this->getParentToLocal());
		if (nfru.isCulling(*boundingVolume))
			return;
		
		if (visualizeBoundingVolume)
			addSpatialVisualizerToScene(scene);
		
		fillScene(fru, scene);
		for (const Spatial *child = getFirstChild(); child; child = child->getNextSibling())
			child->recursiveFillScene(nfru, scene);
	}
	
	void Spatial::recursiveFillNewScene(const CullingVolumesSet &cullingVolumes, newProcessedScene &scene) const
	{
		//TODO write this method
	}

	ProcessedScene *Spatial::getScene(const Camera &camera) const
	{
		Frustum<float> nfru = camera.getFrustum();
		nfru.transform(camera.getWorldToLocalR());
		return getScene(nfru);
	}

	ProcessedScene *Spatial::getScene(const Frustum<float> &frustum) const
	{
		ProcessedScene *scene = new ProcessedScene();
		recursiveFillScene(frustum, *scene);
		return scene;
	}
	
	/// 4 metody poniżej słóżą wyłącznie wizualizacji
	
	void Spatial::setVisualizeBoudingVolume(bool value)
	{
		this->visualizeBoundingVolume = value;
	}
	
	bool Spatial::getVisualizeBoundingVolume() const
	{
		return this->visualizeBoundingVolume;
	}
	
	
	void Spatial::addSpatialVisualizerToScene(ProcessedScene &scene) const
	{
		switch (boundingVolume->getType())
		{
			case BoundingVolumeType::AABB: addSpatialVisualizerAABB(scene, static_cast<const AABB<float> &> (*boundingVolume));
				break;
			case BoundingVolumeType::Sphere: addSpatialVisualizerSphere(scene, static_cast<const Sphere<float> &> (*boundingVolume));
				break;
			default: HASSERT(false);
		}
	}

	void Spatial::addSpatialVisualizerAABB(ProcessedScene &scene, const AABB<float> &aabb) const
	{
		line3f lines[12] = {
			line3f(vector3f(aabb.xmin, aabb.ymin, aabb.zmin), vector3f()),
		};
		float x[] = {aabb.xmin, aabb.xmax};
		float y[] = {aabb.ymin, aabb.ymax};
		float z[] = {aabb.zmin, aabb.zmax};
		auto makeVector = [&](unsigned mask){
			return vector3f(x[mask & 1], y[mask >> 1 & 1], z[mask >> 2 & 1]);
		};
		for (uint i = 0, l = 0; i != 8; i++)
			for (uint j = 0; j < 3; j++)
				if (i & (1u << j))
				{
					HASSERT(l < 12);
					lines[l++] = line3f(makeVector(i), makeVector(i ^ (1u << j)));
				}
		GeometryTask task;
		task.transformation = getWorldToLocalR();
		task.shaderType = ShaderType::Lines;
		task.shaderArgs = shared(new ShaderArgs<line3f> (lines));
		task.useLights = false;
		scene.geometry.push_back(task);
	}

	void Spatial::addSpatialVisualizerSphere(ProcessedScene &scene, const Sphere<float> &sphere) const
	{
		const int res = 10;
		std::vector<vector3f> points;
		float a = float(M_PI / (2 * (res + 1)));
		for (int yi = -res; yi <= res; yi++)
		{
			float y = sin(yi * a) * sphere.radius,
				r = cos(yi * a) * sphere.radius;
			for (int xi = 0; xi < 4 * (res + 1); xi++)
			{
				float x = sin(xi * a) * r,
					z = cos(xi * a) * r;
				points.push_back(sphere.center + vector3f(x, y, z));
			}
		}

		GeometryTask task;
		task.transformation = getWorldToLocalR();
		task.shaderType = ShaderType::Points;
		task.shaderArgs = shared(new ShaderArgs<vector3f > (points.data(), points.size()));
		task.useLights = false;
		scene.geometry.push_back(task);
	}
}
