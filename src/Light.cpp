
/* author: michauzo */

#include "Light.h"
#include "vector3.h"
#include "Memory.h"

namespace Habanero
{

	Light::Light(const Color &color, float range) : Spatial(unique(new Spheref(vector3f::zero, range)))
	{
		Range = Spheref(vector3f::zero, range);
		this->color = color;
	}

	void Light::setColor(Color color)
	{
		this->color = color;
	}
	
	void Light::setRange(float range)
	{
		this->Range = Spheref(vector3f::zero, range);
		this->boundingVolume->zero();
		this->initBoundingVolume();
	}

}