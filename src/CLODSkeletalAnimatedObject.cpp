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
#include "CLODSkeletalAnimatedObject.h"

namespace Habanero
{
	CLODSkeletalAnimatedObject::CLODSkeletalAnimatedObject(Specific_ptr<Skeleton>skeleton) :
		Spatial(unique(new AABBf(0, 0, 0, 0, 0, 0))),
		animationSet(skeleton),
		skeleton(skeleton),
		jointSpatials(new Spatial*[skeleton->numJoints]),
		forcedVsplits(0),
		isLODForced(false)
	{
		std::fill(jointSpatials.get(), jointSpatials.get() + skeleton->numJoints, (Spatial *)NULL);
	}

	CLODSkeletalAnimatedObject::CLODSkeletalAnimatedObject(Specific_ptr<CLODSkinnedMesh> mesh) :
		Spatial(mesh->boundingVolume->clone()),
		animationSet(skeleton),
		skeleton(mesh->skeleton),
		jointSpatials(new Spatial*[mesh->skeleton->numJoints]),
		forcedVsplits(0),
		isLODForced(false)
	{
		//Quickfix, �e ... (ka�dy wie) - nie wiadomo czemu bez rzutowania nie dzia�a pod wind�
		std::fill(jointSpatials.get(), jointSpatials.get() + skeleton->numJoints, (Spatial *)NULL);
		addMesh(mesh);
	}

	CLODSkeletalAnimatedObject::~CLODSkeletalAnimatedObject()
	{
		for (uint i = 0; i < skeleton->numJoints; i++)
			delete jointSpatials[i];
	}

	void CLODSkeletalAnimatedObject::initBoundingVolume()
	{
		if (!m_Meshes.empty())
			for (auto it = m_Meshes.begin(); it != m_Meshes.end(); it++)
				boundingVolume->growToContain(*(*it)->boundingVolume);
	}

	void CLODSkeletalAnimatedObject::addMesh(Specific_ptr<CLODSkinnedMesh> mesh)
	{
		HASSERT(mesh->skeleton == skeleton);
		m_Meshes.push_back(mesh);
		if (boundingVolume->growToContain(*mesh->boundingVolume))
			growBV = true;
	}

	uint CLODSkeletalAnimatedObject::getNumMeshes() const
	{
		return m_Meshes.size();
	}

	Specific_ptr<CLODSkinnedMesh> CLODSkeletalAnimatedObject::getMeshAt(uint index) const
	{
		HASSERT(index < m_Meshes.size());
		return m_Meshes[index];
	}

	void CLODSkeletalAnimatedObject::removeMeshAt(uint index)
	{
		m_Meshes.erase(m_Meshes.begin() + index);

		HASSERT(m_Meshes.size() > 0); // jak b�dzie za�lepka, to dopisz� reszt�.
		rebuildBV = true;
	}

	uint CLODSkeletalAnimatedObject::computeLODLevel(const Frustum<float>& frustum) const
	{
		if(isLODForced)
			return forcedVsplits;

		HASSERT(cross(frustum.front().normal, frustum.rear().normal).length() == 0);
		HASSERT(m_Meshes.size() > 0);

		float totalDistance = abs(frustum.front().d - frustum.rear().d) / frustum.front().normal.length();

		const matrix4f& worldToLocalR = getWorldToLocalR();
		vector3f translation(worldToLocalR(3, 0), worldToLocalR(3, 1), worldToLocalR(3, 2));

		float objectDistance = abs(dot(frustum.front().normal, translation) + frustum.front().d) / frustum.front().normal.length(); // fajnie si� skraca dzi�ki temu odleg�o�� od punktu

		// g�upie skalowanie... powinno jako� wp�ywa� na objectDistance, ale �rednio dzia�a, wi�c wy��czam
		//vector3f ox(1, 0, 0);		// zak�adam, �e uniform scaling (jak nie uniform, to i tak nie bardzo wiem, co robi�...)
		//float scaleZ = (worldToLocalR * oz).length();
		//objectDistance /= scaleX;

		uint value = getMeshAt(0)->getNumVertexSplits();	// legal values: integers in range [0, value]
		for(int i = 1; i < m_Meshes.size(); i++)
			value = clamp<uint>(value, 0, getMeshAt(i)->getNumVertexSplits());

		if(value == 0)
			return 0;
	
		if(objectDistance >= totalDistance)
			return 0;

		uint result = value - round((objectDistance / totalDistance) * value);

		return result;
	}

	void CLODSkeletalAnimatedObject::fillScene(const Frustum<float> &fru, ProcessedScene &scene) const
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

		uint lodLevel = computeLODLevel(fru);		// TODO: wiele meshy, ale jeden LODlevel?

		for (uint i = 0; i < getNumMeshes(); i++)
		{
			task.geometry = getMeshAt(i)->getGeometry(lodLevel);
			scene.geometry.push_back(task);
		}
	}

	void CLODSkeletalAnimatedObject::updateSelf(float time, bool parentUpdated)
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

	const IBoundingVolume<float>* CLODSkeletalAnimatedObject::getPickableBoundingVolume() const
	{
		HASSERT(!m_Meshes.empty());
		return m_Meshes[0]->boundingVolume;
	}

	void CLODSkeletalAnimatedObject::attachToJoint(uint index, Spatial *what)
	{
		HASSERT(jointSpatials[index] == NULL);
		jointSpatials[index] = what;
	}

	void CLODSkeletalAnimatedObject::attachToJoint(const char *name, Spatial *what)
	{
		attachToJoint(skeleton->getJointIndex(name), what);
	}

	std::unique_ptr<Spatial> CLODSkeletalAnimatedObject::detachFromJoint(uint jointId)
	{
		Spatial *spatial = jointSpatials[jointId];
		jointSpatials[jointId] = NULL;
		return unique(spatial);
	}

	std::unique_ptr<Spatial> CLODSkeletalAnimatedObject::detachFromJoint(const char *name)
	{
		return detachFromJoint(skeleton->getJointIndex(name));
	}

	bool CLODSkeletalAnimatedObject::getIsLODForced() const
	{
		return isLODForced;
	}

	void CLODSkeletalAnimatedObject::setIsLODForced(bool value)
	{
		isLODForced = value;
	}

	uint CLODSkeletalAnimatedObject::getForcedLevel() const
	{
		return forcedVsplits;
	}

	void CLODSkeletalAnimatedObject::setForcedLevel(uint value)
	{
		value = std::max<uint>(0, value);
		for(uint i = 0; i < m_Meshes.size(); i++)
			value = std::min<uint>(value, m_Meshes[i]->getNumVertexSplits());
		forcedVsplits = value;
	}

	uint CLODSkeletalAnimatedObject::getMaxLevel() const
	{
		uint result = m_Meshes[0]->getNumVertexSplits();
		for(uint i = 1; i < m_Meshes.size(); i++)
			result = std::min<uint>(result, m_Meshes[i]->getNumVertexSplits());
		return result;
	}
}