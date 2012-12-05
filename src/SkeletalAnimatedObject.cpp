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
#include <vector>

#include "Memory.h"
#include "Sphere.h"
#include "ProcessedScene.h"
#include "IBoundingVolume.h"
#include "SkeletalAnimatedObject.h"

#include "AABB.h"
#include <stddef.h>

namespace Habanero
{
	SkeletalAnimatedObject::SkeletalAnimatedObject(Specific_ptr<Skeleton>skeleton) :
		Spatial(unique(new AABBf(0, 0, 0, 0, 0, 0))),
		animationSet(skeleton),
		skeleton(skeleton),
		jointSpatials(new Spatial*[skeleton->numJoints])
	{
		//Quickfix, że ... (każdy wie) - nie wiadomo czemu bez rzutowania nie działa pod windą
		std::fill(jointSpatials.get(), jointSpatials.get() + skeleton->numJoints, (Spatial *)NULL);
	}
	

	SkeletalAnimatedObject::SkeletalAnimatedObject(Specific_ptr<SkinnedMesh> mesh) :
		Spatial(mesh->boundingVolume->clone()),
		animationSet(skeleton),
		skeleton(mesh->skeleton),
		jointSpatials(new Spatial*[mesh->skeleton->numJoints])
	{
		//Quickfix, że ... (każdy wie) - nie wiadomo czemu bez rzutowania nie działa pod windą
		std::fill(jointSpatials.get(), jointSpatials.get() + skeleton->numJoints, (Spatial *)NULL);
		addMesh(mesh);
	}

	SkeletalAnimatedObject::~SkeletalAnimatedObject()
	{
		for (uint i = 0; i < skeleton->numJoints; i++)
			delete jointSpatials[i];
	}

	void SkeletalAnimatedObject::initBoundingVolume()
	{
		if (!m_Meshes.empty())
			for (auto it = m_Meshes.begin(); it != m_Meshes.end(); it++)
				boundingVolume->growToContain(*(*it)->boundingVolume);
	}

	void SkeletalAnimatedObject::addMesh(Specific_ptr<SkinnedMesh> mesh)
	{
		HASSERT(mesh->skeleton == skeleton);
		m_Meshes.push_back(mesh);
		if (boundingVolume->growToContain(*mesh->boundingVolume))
			growBV = true;
	}

	uint SkeletalAnimatedObject::getNumMeshes() const
	{
		return m_Meshes.size();
	}

	Specific_ptr<SkinnedMesh> SkeletalAnimatedObject::getMeshAt(uint index) const
	{
		HASSERT(index < m_Meshes.size());
		return m_Meshes[index];
	}

	void SkeletalAnimatedObject::removeMeshAt(uint index)
	{
		m_Meshes.erase(m_Meshes.begin() + index);

		HASSERT(m_Meshes.size() > 0); // jak będzie zaślepka, to dopiszę resztę.
		rebuildBV = true;
	}

	void SkeletalAnimatedObject::fillScene(const Frustum<float> &fru, ProcessedScene &scene) const
	{
		//TODO: To powinno byc gdzie indziej
		std::vector<matrix4<float> >
			mbuff1(skeleton->numJoints),
			mbuff2(skeleton->numJoints);

		const matrix4f *reversePose = skeleton->getReversePose();
		animationSet.getSkeletonPose((matrix4f*)&mbuff1[0], (matrix4<float>*)&mbuff2[0]);
		for (uint i = 0; i < skeleton->numJoints; i++)
		{
			mbuff1[i] = reversePose[i] * mbuff1[i];
			if (jointSpatials[i])
				jointSpatials[i]->recursiveFillScene(fru, scene);
		}

		GeometryTask task;
		task.shaderArgs = shared(new ShaderArgs<matrix4f>(mbuff1.data(), mbuff1.size()));
		task.useLights = true;
		task.shaderType = ShaderType::SkeletalAnimation;
		task.transformation = getWorldToLocalR();
		for (uint i = 0; i < getNumMeshes(); i++)
		{
			task.geometry = getMeshAt(i)->getGeometry();
			scene.geometry.push_back(task);
		}
	}

	void SkeletalAnimatedObject::updateSelf(float time, bool parentUpdated)
	{
		if (time != 0)
			animationSet.animate(time);
		if (parentUpdated || time != 0)
		{
			std::vector<matrix4<float> >
				mbuff1(skeleton->numJoints),
				mbuff2(skeleton->numJoints);
			const matrix4f *reversePose = skeleton->getReversePose();
			animationSet.getSkeletonPose((matrix4f*)&mbuff1[0], (matrix4<float>*)&mbuff2[0]);
			for (uint i = 0; i < skeleton->numJoints; i++)
				mbuff1[i] = mbuff1[i] * getWorldToLocalR();
			for (uint i = 0; i < skeleton->numJoints; i++)
				if (jointSpatials[i])
				{
					jointSpatials[i]->setParentToLocalR(mbuff1[i]);
					jointSpatials[i]->update(time);
					if (boundingVolume->growToContain(jointSpatials[i]->getBoundingVolume(), jointSpatials[i]->getParentToLocalR()))
						growBV = true;
				}
		}
		else
		{
			for (uint i = 0; i < skeleton->numJoints; i++)
				if (jointSpatials[i])
				{
					jointSpatials[i]->update(0);
					if (boundingVolume->growToContain(jointSpatials[i]->getBoundingVolume(), jointSpatials[i]->getParentToLocalR()))
						growBV = true;
				}
		}
	}

	const IBoundingVolume<float>* SkeletalAnimatedObject::getPickableBoundingVolume() const
	{

		return m_Meshes[0]->boundingVolume;
	}

	void SkeletalAnimatedObject::attachToJoint(uint index, Spatial *what)
	{
		HASSERT(jointSpatials[index] == NULL);
		jointSpatials[index] = what;
	}

	void SkeletalAnimatedObject::attachToJoint(const char *name, Spatial *what)
	{
		attachToJoint(skeleton->getJointIndex(name), what);
	}

	std::unique_ptr<Spatial> SkeletalAnimatedObject::detachFromJoint(uint jointId)
	{
		Spatial *spatial = jointSpatials[jointId];
		jointSpatials[jointId] = NULL;
		return unique(spatial);
	}

	std::unique_ptr<Spatial> SkeletalAnimatedObject::detachFromJoint(const char *name)
	{
		return detachFromJoint(skeleton->getJointIndex(name));
	}
}