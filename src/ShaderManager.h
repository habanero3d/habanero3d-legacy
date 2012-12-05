#pragma once

#include <map>

#include "ShaderFlags.h"
#include "LightShader.h"
#include "ShaderProgram.h"
#include "SkeletalShader.h"

namespace Habanero
{
	class ShaderManager
	{
		friend class OpenGL_Renderer;

	public:
		LightShader *getLightShaderProgram(const ShaderFlags &flags);
		SkeletalShader *getSkeletalShaderProgram(const ShaderFlags &flags);
		ShaderProgram *getNoneShaderProgram() const;

		static ShaderManager & getInstance();
		static ShaderManager * getInstancePtr();

	private:
		ShaderProgram * const noneShader;
		std::map<ShaderFlags, SkeletalShader *> skeletalShaderPrograms;
		std::map<ShaderFlags, LightShader *> staticShaderPrograms;

		ShaderManager();
		~ShaderManager();
		static ShaderManager * singleInstance;
	};

	
}