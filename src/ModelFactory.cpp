#include "ModelFactory.h"
#include "ResourceManager.h"
#include "StaticObject.h"
#include "SkeletalAnimatedObject.h"
#include "DLODStaticObject.h"
#include "DLODSkeletalAnimatedObject.h"
#include "CLODStaticObject.h"
#include "CLODSkeletalAnimatedObject.h"
#include "HeightMapObject.h"

#define CreateObjectMacro(MeshType, ObjectType) \
	Specific_ptr<MeshType> mesh = MeshType ## Manager::getInstance().getResource(model.name); \
	mesh->load(); \
	ret = new ObjectType(mesh)

namespace Habanero
{
	namespace ModelLoading
	{

		Spatial *ModelFactory::GetModel(const Model &model)
		{
			Spatial *ret;
			switch (model.type)
			{
				case Static:
				{
					CreateObjectMacro(StaticMesh, StaticObject);
					break;
				}
				case Animated:
				{
					CreateObjectMacro(SkinnedMesh, SkeletalAnimatedObject);
					break;
				}
				case DLODStatic:
				{
					CreateObjectMacro(DLODStaticMesh, DLODStaticObject);
					break;
				}
				case DLODAnimated:
				{
					CreateObjectMacro(DLODSkinnedMesh, DLODSkeletalAnimatedObject);
					break;
				}
				case CLODStatic:
				{
					CreateObjectMacro(CLODStaticMesh, CLODStaticObject);
					break;
				}
				case CLODAnimated:
				{
					CreateObjectMacro(CLODSkinnedMesh, CLODSkeletalAnimatedObject);
					break;
				}
				case HeightMap:
				{
					CreateObjectMacro(HeightMapMesh, HeightMapObject);
					break;
				}
			}
			return ret;
		}
	
	}
}
