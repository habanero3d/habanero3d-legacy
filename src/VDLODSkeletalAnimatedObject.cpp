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

#include "AABB.h"
#include "Memory.h"
#include "Frustum.h"
#include "HabMath.h"
#include "GeometryTask.h"
#include "ProcessedScene.h"
#include "VDLODSkeletalAnimatedObject.h"

namespace Habanero
{
	VDLODSkeletalAnimatedObject::VDLODSkeletalAnimatedObject(Specific_ptr<Skeleton>skeleton) :
		Spatial(unique(new AABBf(0, 0, 0, 0, 0, 0))),
		animationSet(skeleton),
		skeleton(skeleton),
		jointSpatials(new Spatial*[skeleton->numJoints])
	{
		std::fill(jointSpatials.get(), jointSpatials.get() + skeleton->numJoints, (Spatial *)NULL);
	}

	VDLODSkeletalAnimatedObject::VDLODSkeletalAnimatedObject(Specific_ptr<VDLODSkinnedMesh> mesh) :
		Spatial(mesh->boundingVolume->clone()),
		animationSet(skeleton),
		skeleton(mesh->skeleton),
		jointSpatials(new Spatial*[mesh->skeleton->numJoints])
	{
		//Quickfix, �e ... (ka�dy wie) - nie wiadomo czemu bez rzutowania nie dzia�a pod wind�
		std::fill(jointSpatials.get(), jointSpatials.get() + skeleton->numJoints, (Spatial *)NULL);
		addMesh(mesh);
	}

	VDLODSkeletalAnimatedObject::~VDLODSkeletalAnimatedObject()
	{
		for (uint i = 0; i < skeleton->numJoints; i++)
			delete jointSpatials[i];
	}

	void VDLODSkeletalAnimatedObject::initBoundingVolume()
	{
		if (!m_Meshes.empty())
			for (auto it = m_Meshes.begin(); it != m_Meshes.end(); it++)
				boundingVolume->growToContain(*(*it)->boundingVolume);
	}

	void VDLODSkeletalAnimatedObject::addMesh(Specific_ptr<VDLODSkinnedMesh> mesh)
	{
		HASSERT(mesh->skeleton == skeleton);
		m_Meshes.push_back(mesh);
		if (boundingVolume->growToContain(*mesh->boundingVolume))
			growBV = true;
	}

	uint VDLODSkeletalAnimatedObject::getNumMeshes() const
	{
		return m_Meshes.size();
	}

	Specific_ptr<VDLODSkinnedMesh> VDLODSkeletalAnimatedObject::getMeshAt(uint index) const
	{
		HASSERT(index < m_Meshes.size());
		return m_Meshes[index];
	}

	void VDLODSkeletalAnimatedObject::removeMeshAt(uint index)
	{
		m_Meshes.erase(m_Meshes.begin() + index);

		HASSERT(m_Meshes.size() > 0); // jak b�dzie za�lepka, to dopisz� reszt�.
		rebuildBV = true;
	}

	void VDLODSkeletalAnimatedObject::fillScene(const Frustum<float> &fru, ProcessedScene &scene) const
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
			task.geometry = getMeshAt(i)->getGeometry(fru, getParentToLocal());
			scene.geometry.push_back(task);
		}
	}

	void VDLODSkeletalAnimatedObject::updateSelf(float time, bool parentUpdated)
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

	const IBoundingVolume<float>* VDLODSkeletalAnimatedObject::getPickableBoundingVolume() const
	{
		HASSERT(!m_Meshes.empty());
		return m_Meshes[0]->boundingVolume;
	}

	void VDLODSkeletalAnimatedObject::attachToJoint(uint index, Spatial *what)
	{
		HASSERT(jointSpatials[index] == NULL);
		jointSpatials[index] = what;
	}

	void VDLODSkeletalAnimatedObject::attachToJoint(const char *name, Spatial *what)
	{
		attachToJoint(skeleton->getJointIndex(name), what);
	}

	std::unique_ptr<Spatial> VDLODSkeletalAnimatedObject::detachFromJoint(uint jointId)
	{
		Spatial *spatial = jointSpatials[jointId];
		jointSpatials[jointId] = NULL;
		return unique(spatial);
	}

	std::unique_ptr<Spatial> VDLODSkeletalAnimatedObject::detachFromJoint(const char *name)
	{
		return detachFromJoint(skeleton->getJointIndex(name));
	}
}
