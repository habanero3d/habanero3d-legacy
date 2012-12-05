#pragma once

/* author: michauzo */

#include "Spatial.h"
#include "Color.h"
#include "Sphere.h"

namespace Habanero
{
	class Light : public Spatial
	{
	protected:
		Sphere<float> Range;
		Color color;
		Light(const Color &color, float range);
	public:
		void setColor(Color color);
		void setRange(float range);
	};
}