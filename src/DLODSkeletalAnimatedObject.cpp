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

#include "AABB.h"
#include "Memory.h"
#include "Frustum.h"
#include "HabMath.h"
#include "GeometryTask.h"
#include "ProcessedScene.h"
#include "DLODSkeletalAnimatedObject.h"

namespace Habanero
{
	DLODSkeletalAnimatedObject::DLODSkeletalAnimatedObject(Specific_ptr<Skeleton>skeleton) :
		Spatial(unique(new AABBf(0, 0, 0, 0, 0, 0))),
		animationSet(skeleton),
		skeleton(skeleton),
		jointSpatials(new Spatial*[skeleton->numJoints]),
		forcedLevel(0),
		isLODForced(false)
	{
		//Quickfix, że ... (każdy wie) - nie wiadomo czemu bez rzutowania nie działa pod windą
		std::fill(jointSpatials.get(), jointSpatials.get() + skeleton->numJoints, (Spatial *)NULL);
	}

	DLODSkeletalAnimatedObject::DLODSkeletalAnimatedObject(Specific_ptr<DLODSkinnedMesh> mesh) :
		Spatial(mesh->boundingVolume->clone()),
		animationSet(skeleton),
		skeleton(mesh->skeleton),
		jointSpatials(new Spatial*[mesh->skeleton->numJoints]),
		forcedLevel(0),
		isLODForced(false)
	{
		//Quickfix, że ... (każdy wie) - nie wiadomo czemu bez rzutowania nie działa pod windą
		std::fill(jointSpatials.get(), jointSpatials.get() + skeleton->numJoints, (Spatial *)NULL);
		addMesh(mesh);
	}

	DLODSkeletalAnimatedObject::~DLODSkeletalAnimatedObject()
	{
		for (uint i = 0; i < skeleton->numJoints; i++)
			delete jointSpatials[i];
	}

	void DLODSkeletalAnimatedObject::initBoundingVolume()
	{
		if (!m_Meshes.empty())
			for (auto it = m_Meshes.begin(); it != m_Meshes.end(); it++)
				boundingVolume->growToContain(*(*it)->boundingVolume);
	}

	void DLODSkeletalAnimatedObject::addMesh(Specific_ptr<DLODSkinnedMesh> mesh)
	{
		HASSERT(mesh->skeleton == skeleton);
		m_Meshes.push_back(mesh);
		if (boundingVolume->growToContain(*mesh->boundingVolume))
			growBV = true;
	}

	uint DLODSkeletalAnimatedObject::getNumMeshes() const
	{
		return m_Meshes.size();
	}

	Specific_ptr<DLODSkinnedMesh> DLODSkeletalAnimatedObject::getMeshAt(uint index) const
	{
		HASSERT(index < m_Meshes.size());
		return m_Meshes[index];
	}

	void DLODSkeletalAnimatedObject::removeMeshAt(uint index)
	{
		m_Meshes.erase(m_Meshes.begin() + index);

		HASSERT(m_Meshes.size() > 0); // jak będzie zaślepka, to dopiszę resztę.
		rebuildBV = true;
	}

	float DLODSkeletalAnimatedObject::computeLODLevel(const Frustum<float>& frustum) const
	{
		if(isLODForced)
			return forcedLevel;

		HASSERT(cross(frustum.front().normal, frustum.rear().normal).length() == 0);
		//HASSERT(frustum.front.normal.length() == 1);
		HASSERT(m_Meshes.size() > 0);

		float totalDistance = abs(frustum.front().d - frustum.rear().d) / frustum.front().normal.length();

		const matrix4f& worldToLocalR = getWorldToLocalR();
		vector3f translation(worldToLocalR(3, 0), worldToLocalR(3, 1), worldToLocalR(3, 2));

		float objectDistance = abs(dot(frustum.front().normal, translation) + frustum.front().d) / frustum.front().normal.length(); // fajnie się skraca dzięki temu odległość od punktu

		// głupie skalowanie... powinno jakoś wpływać na objectDistance, ale średnio działa, więc wyłączam
		//vector3f ox(1, 0, 0);		// zakładam, że uniform scaling (jak nie uniform, to i tak nie bardzo wiem, co robić...)
		//float scaleX = (worldToLocalR * ox).length();
		//objectDistance /= scaleX;

		uint maxValue = getMeshAt(0)->getNumLODLevels() - 1;	// max possible value
		for(int i = 1; i < m_Meshes.size(); i++)
			maxValue = clamp<uint>(maxValue, 0, getMeshAt(i)->getNumLODLevels() - 1);

		if(maxValue == 0)
			return 0;
		
		if(objectDistance > totalDistance)
			return maxValue;

		return floor(maxValue * objectDistance / totalDistance);
	}

	void DLODSkeletalAnimatedObject::fillScene(const Frustum<float> &fru, ProcessedScene &scene) const
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

		float lodLevel = computeLODLevel(fru);		// TODO: wiele meshy, ale jeden LODlevel?

		for (uint i = 0; i < getNumMeshes(); i++)
		{
			task.geometry = getMeshAt(i)->getGeometry(lodLevel);
			scene.geometry.push_back(task);
		}
	}

	void DLODSkeletalAnimatedObject::updateSelf(float time, bool parentUpdated)
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

	const IBoundingVolume<float>* DLODSkeletalAnimatedObject::getPickableBoundingVolume() const
	{

		return m_Meshes[0]->boundingVolume;
	}

	void DLODSkeletalAnimatedObject::attachToJoint(uint index, Spatial *what)
	{
		HASSERT(jointSpatials[index] == NULL);
		jointSpatials[index] = what;
	}

	void DLODSkeletalAnimatedObject::attachToJoint(const char *name, Spatial *what)
	{
		attachToJoint(skeleton->getJointIndex(name), what);
	}

	std::unique_ptr<Spatial> DLODSkeletalAnimatedObject::detachFromJoint(uint jointId)
	{
		Spatial *spatial = jointSpatials[jointId];
		jointSpatials[jointId] = NULL;
		return unique(spatial);
	}

	std::unique_ptr<Spatial> DLODSkeletalAnimatedObject::detachFromJoint(const char *name)
	{
		return detachFromJoint(skeleton->getJointIndex(name));
	}

	bool DLODSkeletalAnimatedObject::getIsLODForced() const
	{
		return isLODForced;
	}

	void DLODSkeletalAnimatedObject::setIsLODForced(bool value)
	{
		isLODForced = value;
	}

	uint DLODSkeletalAnimatedObject::getForcedLevel() const
	{
		return forcedLevel;
	}

	void DLODSkeletalAnimatedObject::setForcedLevel(uint value)
	{
		for(int i = 0; i < m_Meshes.size(); i++)
			value = clamp<uint>(value, 0, getMeshAt(i)->getNumLODLevels() - 1);
		forcedLevel = value;
	}
}