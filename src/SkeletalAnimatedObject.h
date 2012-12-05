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
@author akuda
*/

#pragma once
#include <memory>
#include <vector>

#include "Spatial.h"
#include "Skeleton.h"
#include "SkinnedMesh.h"
#include "SkeletalAnimationState.h"
#include "SkeletalAnimationStateSet.h"
#include "Mapping.h"

namespace Habanero
{
	/*!
		Klasa reprezentuje obiekt animowany szkieletowo.
		Obiekt na SCENIE, a nie zasoby z których się składa.
		To jest wielo-instancja zasobów, nic więcej.
		Do jednego szkieletu może być przypięte wiele meshów - np. ubrania w stylu Morrowind :)
		*/
	class SkeletalAnimatedObject : public Spatial
	{
	private:
		std::vector<Specific_ptr<SkinnedMesh> > m_Meshes;
		std::unique_ptr<Spatial*[]> jointSpatials;
		void attachToJoint(SkeletonJoint &joint, Spatial *what);

	protected:
		void fillScene(const Frustum<float>&, ProcessedScene &scene) const;
		void updateSelf(float time, bool parentUpdated);
		void initBoundingVolume();

	public:
		const Specific_ptr<Skeleton> skeleton;
		SkeletalAnimationStateSet animationSet;

		/*!
			@brief konstruktor bez-meshowy, uzywa pustego BV
			@param skeleton ptr na szkielet
			*/
		SkeletalAnimatedObject(Specific_ptr<Skeleton> skeleton);

		/*!
			@brief konstruktor bez-meshowy, wymaga podania BV, bo nie ma jak go policzyć sam.
			@param mesh ptr na pierwszy SkinnedMesh, od którego zostanie skopiowany BV;
			@param boundingVolume BV, który opisuje ten obiekt (będzie rósł w miarę dodawania meshy)
			*/
		SkeletalAnimatedObject(Specific_ptr<SkinnedMesh> mesh);

		~SkeletalAnimatedObject();

		
		/*!
			@brief dodaje nowego SkinnedMesh'a do obiektu animowanego, powiększa BV
			@param mesh ptr na nowego SkinnedMesh'a
			*/
		void addMesh(Specific_ptr<SkinnedMesh> mesh);

		/*!
			@brief podaje liczbę SkinnedMesh'y tworzących ten obiekt
			*/
		uint getNumMeshes() const;

		Specific_ptr<SkinnedMesh> getMeshAt(uint index) const;

		/*!
			@brief odłącza SkinnedMesh'a z obiektu animowanego, odbudowuje BV iterując po pozostałych meshach, albo się wywala.
			*/
		void removeMeshAt(uint index);

		void attachToJoint(uint jointId, Spatial *what);
		void attachToJoint(const char *name, Spatial *what);
		std::unique_ptr<Spatial> detachFromJoint(uint jointId);
		std::unique_ptr<Spatial> detachFromJoint(const char *name);

		const IBoundingVolume<float>* getPickableBoundingVolume() const;
	};
}