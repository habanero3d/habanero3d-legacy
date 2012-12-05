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


#include "ShaderProgram.h"
#include "File.h"
#include "Exception.h"
#include "StringUtil.h"
#include "Memory.h"
#include "Loggers.h"

#include <cstring>

using std::string;

namespace Habanero
{
	ShaderProgram::ShaderProgram()
	{
		p = f = v = 0;
	}

	ShaderProgram::ShaderProgram(ShaderProgram &other)
	{
		p = other.p;
		f = other.f;
		v = other.v;
		other.p = other.f = other.v = 0;
	}

	ShaderProgram::ShaderProgram(const char *vertex, const char *fragment, const ShaderFlags &flags)
	{
		File::uptr vs, fs;		
		
		v = glCreateShader(GL_VERTEX_SHADER);
		f = glCreateShader(GL_FRAGMENT_SHADER);
	
		uint64 size;

		vs = File::map(vertex, File::Read, &size);
		std::unique_ptr<char[]> vs1(new char[size + 1]);
		memcpy(vs1.get(), vs.get(), size * sizeof(char));
		vs1[size] = 0;

		fs = File::map(fragment, File::Read, &size);
		std::unique_ptr<char[]> fs1(new char[size + 1]);
		memcpy(fs1.get(), fs.get(), size * sizeof(char));
		fs1[size] = 0;

		//TODO: Może troszkę to ulepszyć... (Chodzi o to, że header trzeba dokleić po #version)
		const char * vv = vs1.get();
		std::string fscode = fs1.get();
		size_t ver = fscode.find("#version");
		ver = fscode.find("\n", ver);
		std::string ffs = fscode.substr(0, ver) + "\n" + flags.getHeader() + fscode.substr(ver + 1);
		const char * ff = ffs.c_str();

		glShaderSource(v, 1, &vv, NULL);
		glShaderSource(f, 1, &ff, NULL);

		GLint status;
		glCompileShader(v);
		glGetShaderiv(v, GL_INFO_LOG_LENGTH, &status);
		std::unique_ptr<GLchar[]> buffer(new GLchar[status]);
		glGetShaderInfoLog(v, status, &status, buffer.get());
		glGetShaderiv(v, GL_COMPILE_STATUS, &status);
		if(status != GL_TRUE)
			RAISE(ShaderException, "Error while compiling Vertex Shader!", buffer.get());
		else
			LOG(errorLogger, "Vertex Shader Info Log:%s\n", buffer.get());

		glCompileShader(f);
		glGetShaderiv(f, GL_INFO_LOG_LENGTH, &status);
		buffer = std::unique_ptr<GLchar[]>(new GLchar[status]);
		glGetShaderInfoLog(f, status, &status, buffer.get());
		glGetShaderiv(f, GL_COMPILE_STATUS, &status);
		if(status != GL_TRUE)
			RAISE(ShaderException, "Error while compiling Fragment Shader!", buffer.get());
		else
			LOG(errorLogger, "Fragment Shader Info Log:%s\n", buffer.get());

		p = glCreateProgram();

		glAttachShader(p, v);
		glAttachShader(p, f);
		
		glLinkProgram(p);

		glValidateProgram(p);

		glGetProgramiv(p, GL_INFO_LOG_LENGTH, &status);
		buffer = std::unique_ptr<GLchar[]>(new GLchar[status]);
		glGetProgramInfoLog(p, status, &status, buffer.get());
		glGetProgramiv(p, GL_VALIDATE_STATUS, &status);
		if(status != GL_TRUE)
			RAISE(ShaderException, "Error while linking / validating Shader Program!", buffer.get());
		else
			LOG(errorLogger, "Shader Program Info Log:%s\n", buffer.get());

		HASSERT(p && v && f);
	}


	ShaderProgram::~ShaderProgram(void)
	{
		if(v)
		{
			glDetachShader(p, v);
			glDeleteShader(v);
		}
		if(f)
		{
			glDetachShader(p, f);
			glDeleteShader(f);
		}
		if(p)
			glDeleteProgram(p);
	}

	void ShaderProgram::enable()
	{
		glUseProgram(p);
	}

	void ShaderProgram::disable()
	{
		glUseProgram(0);
	}

	GLuint ShaderProgram::getProgramHandler() const
	{
		return p;
	}

	ShaderException::ShaderException(const string &msg,
				  const std::string &det,
				  const char *fileName,
				  const char *functionName,
				  uint lineNumber) : Exception(msg, fileName, functionName, lineNumber), details(det)
	{
	}

	ShaderException::~ShaderException()
	{
	}

	std::string ShaderException::describe() const
	{
		if (fileName != NULL)
			return format("%s at %s:%u(%s)\nShader Info Log: %s", msg.c_str(), fileName, lineNumber, functionName, details.c_str());
		else
			return msg;
	}

}
