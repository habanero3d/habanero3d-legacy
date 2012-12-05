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

#include "Node.h"
#include "matrix4.h"
#include "IBoundingVolume.h"

#include <vector>

namespace Habanero
{
	template <typename Real> class Frustum;
	template <typename Real> class AABB;
	template <typename Real> class Sphere;
	
	class Camera;
	struct ProcessedScene;

	class CullingVolumesSet;
	class newProcessedScene;
	
	class Spatial : public Node<Spatial>
	{
	private:
		matrix4f parentToLocalR;
		matrix4f parentToLocal;
		matrix4f worldToLocal;
		matrix4f worldToLocalR;
		bool dirty, dirtyR, parentChanged;

		bool visualizeBoundingVolume;
		bool _isShadowCaster;
		bool _isLightSource;
		
		void updateTreeInternal(float time, bool updateTransform);
		void rebuildBoundingVolume();
		void growBoundingVolume();

	protected:
		bool growBV, rebuildBV;
		std::unique_ptr<IBoundingVolume<float >> boundingVolume;
		
		

		virtual void updateSelf(float time, bool parentUpdated)
		{
		};

		virtual void initBoundingVolume()
		{
		}
		// Ze wzgledu na kamere domyslnie nic nie robi

		virtual void fillScene(const Frustum<float>&, ProcessedScene &scene) const
		{
		}
		
		virtual void fillNewScene(const CullingVolumesSet &cullingVolumes, newProcessedScene &scene) const
		{
		}
	
	public:
		inline bool isLightSource() const;
		inline bool isShadowCaster() const;
		/*
		 TODO: to tylko funkcja pomocnicza na czas pisania renderera. trzeba
		 ją wywalić!
		 */
		std::vector<const Spatial*> getLights(std::vector<const Spatial*> &lights) const;

	public:
		Spatial(std::unique_ptr<IBoundingVolume<float >> && boundingVolume, bool isLightSource = false);

		void setVisualizeBoudingVolume(bool value);
		bool getVisualizeBoundingVolume() const;
		
		void attach(Spatial *what);
		void detach();

		//! Uaktualnia wszystkie cache'owane macierze i BV w poddrzewie, propaguje zmianę czasu
		void update(float time);

		const matrix4f& getWorldToLocal() const;
		const matrix4f& getWorldToLocalR() const;
		const matrix4f& getParentToLocal() const;
		const matrix4f& getParentToLocalR() const;

		void setParentToLocal(const matrix4f &m);
		void setParentToLocalR(const matrix4f &m);

		const IBoundingVolume<float>& getBoundingVolume() const;
		virtual const IBoundingVolume<float>* getPickableBoundingVolume() const;

		void recursiveFillScene(const Frustum<float> &fru, ProcessedScene &scene) const;
		void recursiveFillNewScene(const CullingVolumesSet &cullingVolumes, newProcessedScene &scene) const;
		
		ProcessedScene *getScene(const Camera &camera) const;
		ProcessedScene *getScene(const Frustum<float> &fru) const;

	protected:
		void addSpatialVisualizerToScene(ProcessedScene &scene) const;
		
	private:
		void addSpatialVisualizerAABB(ProcessedScene &scene, const AABB<float> &aabb) const;
		void addSpatialVisualizerSphere(ProcessedScene &scene, const Sphere<float> &sphere) const;
	};
}
