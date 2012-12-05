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
#include <algorithm>

#include "SkeletalAnimationStateSet.h"
#include "matrix4.h"
#include "Memory.h"
#include "Specific_ptr.h"

namespace Habanero
{
	SkeletalAnimationStateSet::SkeletalAnimationStateSet(Specific_ptr<Skeleton > skeleton) :
		skeleton(skeleton)
	{
		animationStates = uninitializedAlloc<SkeletalAnimationState>(skeleton->numAnimations);
		for(uint i = 0; i < skeleton->numAnimations; i++)
			new(&animationStates[i]) SkeletalAnimationState(skeleton->getAnimation(i));
	}

	SkeletalAnimationStateSet::~SkeletalAnimationStateSet()
	{
		uninitializedDelete(animationStates, skeleton->numAnimations);
	}

	void SkeletalAnimationStateSet::animate(float deltaTime)
	{
		for (uint i = 0; i < skeleton->numAnimations; i++)
			animationStates[i].setTimePosition(animationStates[i].getTimePosition() + deltaTime);
	}

	SkeletalAnimationState& SkeletalAnimationStateSet::getAnimationState(uint index)
	{
		return animationStates[index];
	}

	const SkeletalAnimationState& SkeletalAnimationStateSet::getAnimationState(uint index) const
	{
		return animationStates[index];
	}

	const SkeletalAnimationState& SkeletalAnimationStateSet::getAnimationState(const char *name) const
	{
		return animationStates[skeleton->getAnimation(name).getIndex()];
	}

	SkeletalAnimationState& SkeletalAnimationStateSet::getAnimationState(const char *name)
	{
		return animationStates[skeleton->getAnimation(name).getIndex()];
	}

	void SkeletalAnimationStateSet::getSkeletonPose(matrix4<float> *finalJointPoses, matrix4<float> *jointPoses) const
	{
		//HASSERT(skeleton->numAnimations >= 1);
		float weight = 0;
		std::fill(finalJointPoses, finalJointPoses + skeleton->numJoints, matrix4<float>::zero);
		for (uint i = 0; i < skeleton->numAnimations; i++)
			if (animationStates[i].weight != 0.0)
			{
				weight += animationStates[i].weight;
				animationStates[i].getSkeletonPose(jointPoses);
				for (uint j = 0; j < skeleton->numJoints; j++)
				{
					jointPoses[j] *= animationStates[i].weight;
					finalJointPoses[j] += jointPoses[j];
				}
			}
		
		if (weight != 0)
		{
			weight = 1.0f / weight;
			for (uint i = 0; i < skeleton->numJoints; i++)
				finalJointPoses[i] *= weight;
		}
		//TODO: Tego raczej nie powinno byc, ale sie przyda dla debugu - bindpose
		else
		{
			const matrix4<float> *reversePose = skeleton->getReversePose();
			for (uint i = 0; i < skeleton->numJoints; i++)
				finalJointPoses[i] = reversePose[i].inversed();
		}
	}
}