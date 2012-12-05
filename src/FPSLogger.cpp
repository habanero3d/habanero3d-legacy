#include "Memory.h"
#include "Sphere.h"
#include "vector3.h"
#include "FPSLogger.h"

namespace Habanero
{
	FPSLogger::FPSLogger(const std::string& filename, float range, float time)
		: Spatial(unique(new Spheref(vector3f::zero, range))),
		maxTime(time), recordedTime(0), recordedFrames(0), fileStream(filename)
	{
	}

	void FPSLogger::updateSelf(float time, bool parentUpdated)
	{
		recordedTime += time;
		recordedFrames++;

		if(recordedTime > maxTime)
		{
			fileStream << "FPS:\t" << recordedFrames << std::endl;
			recordedTime -= maxTime;
			recordedFrames = 0;
		}
	}
}