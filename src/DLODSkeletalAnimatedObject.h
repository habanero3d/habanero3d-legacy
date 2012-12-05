/*
Copyright 2009-2012 Andrzej Skalski, Piotr Kufel, Piotr Bia�ecki, Micha� �ochowski, and Micha� Szczepaniak
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

#include <vector>

#include "Specific_ptr.h"
#include "Spatial.h"
#include "DLODSkinnedMesh.h"
#include "SkeletalAnimationStateSet.h"

namespace Habanero
{
	class DLODSkeletalAnimatedObject : public Spatial
	{
	private:
		std::vector<Specific_ptr<DLODSkinnedMesh> > m_Meshes;
		std::unique_ptr<Spatial*[]> jointSpatials;
		void attachToJoint(SkeletonJoint &joint, Spatial *what);

		bool isLODForced;
		uint forcedLevel;

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
		DLODSkeletalAnimatedObject(Specific_ptr<Skeleton> skeleton);

		DLODSkeletalAnimatedObject(Specific_ptr<DLODSkinnedMesh> mesh);

		~DLODSkeletalAnimatedObject();

		
		/*!
			@brief dodaje nowego DLODSkinnedMesh'a do obiektu animowanego, powi�ksza BV
			@param mesh ptr na nowego SkinnedMesh'a
			*/
		void addMesh(Specific_ptr<DLODSkinnedMesh> mesh);

		/*!
			@brief podaje liczb� SkinnedMesh'y tworz�cych ten obiekt
			*/
		uint getNumMeshes() const;

		Specific_ptr<DLODSkinnedMesh> getMeshAt(uint index) const;

		/*!
			@brief od��cza SkinnedMesh'a z obiektu animowanego, odbudowuje BV iteruj�c po pozosta�ych meshach, albo si� wywala.
			*/
		void removeMeshAt(uint index);

		void attachToJoint(uint jointId, Spatial *what);
		void attachToJoint(const char *name, Spatial *what);
		std::unique_ptr<Spatial> detachFromJoint(uint jointId);
		std::unique_ptr<Spatial> detachFromJoint(const char *name);

		const IBoundingVolume<float>* getPickableBoundingVolume() const;

		float computeLODLevel(const Frustum<float>& frustum) const;

		bool getIsLODForced() const;
		void setIsLODForced(bool value);

		uint getForcedLevel() const;
		void setForcedLevel(uint value);
	};
}