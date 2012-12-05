#include <vector>

#include "Memory.h"
#include "SkeletonVisualizer.h"
#include "SkeletalAnimatedObject.h"
#include "GeometryTask.h"
#include "ProcessedScene.h"

namespace Habanero
{
    SkeletonVisualizer::SkeletonVisualizer(SkeletalAnimatedObject &skeletonToDraw): Spatial(skeletonToDraw.getBoundingVolume().clone()), skeletonToDraw(&skeletonToDraw)
    {
        skeletonToDraw.attach(this);
    }

    void SkeletonVisualizer::initBoundingVolume()
	{
        boundingVolume->growToContain(skeletonToDraw->getBoundingVolume());
	}

    void SkeletonVisualizer::fillScene(const Frustum<float>&, ProcessedScene &scene) const
    {
        typedef std::pair<vector3f, vector3f> Line;
        std::vector<Line> lines;
        const Specific_ptr<Skeleton>skeleton = skeletonToDraw->skeleton;
        
        matrix4f *mbuff1, *mbuff2;
        mbuff1 = new matrix4f[skeleton->numJoints];
        mbuff2 = new matrix4f[skeleton->numJoints];
        skeletonToDraw->animationSet.getSkeletonPose(mbuff1, mbuff2);
        std::vector<vector3f> points(skeleton->numJoints);
        for (uint i = 0; i < skeleton->numJoints; i++)
        {
            // tak naprawdê wyci¹ga translacjê (ostateczna pozycja jointa)
            points[i] = mbuff1[i].transformVertex(vector3f::zero);
        }
        for (uint i = 0; i < skeleton->numJoints; i++)
        {
            if (skeleton->getJoint(i).parent != SkeletonJoint::noParent)
                lines.push_back(Line(points[skeleton->getJoint(i).parent], points[i]));
        }

        GeometryTask task;
        task.transformation = getWorldToLocalR();
		task.shaderType = ShaderType::Lines;
		task.shaderArgs = shared(new ShaderArgs<Line>(lines.data(), lines.size()));
		task.useLights = false;
		scene.geometry.push_back(task);

        GeometryTask task2;
        task2.transformation = getWorldToLocalR();
		task2.shaderType = ShaderType::Points;
		task2.shaderArgs = shared(new ShaderArgs<vector3f>(points.data(), points.size()));
		task2.useLights = false;
		scene.geometry.push_back(task2);

        delete [] mbuff1;
        delete [] mbuff2;
    }
}