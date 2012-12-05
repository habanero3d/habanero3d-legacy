#include "ShaderManager.h"

namespace Habanero
{
	ShaderManager *ShaderManager::singleInstance = NULL;

	ShaderManager::ShaderManager()
		: noneShader(new ShaderProgram)
	{
		singleInstance = this;
		// TODO kompilowaæ shadery i wrzucaæ do map (na razie kompilowane leniwie - czy chcemy to zmieniaæ? )
	}

	ShaderManager::~ShaderManager()
	{
		delete noneShader;
		for (auto it = staticShaderPrograms.begin(); it != staticShaderPrograms.end(); ++it)
			delete it->second;
		for (auto it = skeletalShaderPrograms.begin(); it != skeletalShaderPrograms.end(); ++it)
			delete it->second;
	}

	LightShader *ShaderManager::getLightShaderProgram(const ShaderFlags &flags)
	{			
		if (staticShaderPrograms.count(flags) != 0)
			return staticShaderPrograms[flags];
		staticShaderPrograms[flags] = new LightShader("Shaders/vertex.glsl", "Shaders/fragment.glsl", flags);
		return staticShaderPrograms.at(flags);
	}

	SkeletalShader *ShaderManager::getSkeletalShaderProgram(const ShaderFlags &flags)
	{
		if (skeletalShaderPrograms.count(flags) != 0)
			return skeletalShaderPrograms[flags];
		skeletalShaderPrograms[flags] = new SkeletalShader("Shaders/vertex-skinning.glsl", "Shaders/fragment.glsl", ShaderFlags(flags));
		return skeletalShaderPrograms.at(flags);
	}

	ShaderProgram *ShaderManager::getNoneShaderProgram() const
	{
		return noneShader;
	}

	ShaderManager & ShaderManager::getInstance()
	{
		return *singleInstance;
	}

	ShaderManager * ShaderManager::getInstancePtr()
	{
		return singleInstance;
	}
}