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


#pragma once

#include "Enum.h"

namespace Habanero
{
	ENUM(ShaderType, ShaderNone = 0, StaticObject, SkeletalAnimation, Lines, Points);

	class ShaderArgsBase
	{
	private:
		void *data;
		size_t size;
	public:
		ShaderArgsBase(const void *data, size_t size);
		~ShaderArgsBase();

		const void *getData() const;
		size_t getSize() const;
	};

	template <typename T>
	class ShaderArgs : public ShaderArgsBase
	{
	public:
		ShaderArgs(const T *data, uint count) : ShaderArgsBase(data, count * sizeof(T)) {}
		ShaderArgs(const T &obj) : ShaderArgsBase(&obj, sizeof(obj)) {}
		template <size_t Count>
		ShaderArgs(const T (&data)[Count]) : ShaderArgsBase(data, Count * sizeof(T)) {}
		const T *getData() const { return (T*)ShaderArgsBase::getData(); }
		size_t getCount() const { return ShaderArgsBase::getSize() / sizeof(T); }
	};
}
