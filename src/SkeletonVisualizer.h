#pragma once
#include <memory>

#include "Spatial.h"
#include "SkeletalAnimatedObject.h"

namespace Habanero
{

	class SkeletonVisualizer : public Spatial
	{
	private:
        const SkeletalAnimatedObject *skeletonToDraw;

	protected:
		void fillScene(const Frustum<float>&, ProcessedScene &scene) const;
		void initBoundingVolume();
	public:
		SkeletonVisualizer(SkeletalAnimatedObject &skeletonToDraw);
	};
}