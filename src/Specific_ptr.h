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

/*
Author: Andrzej "akuda" Skalski
 */

#pragma once

#include "GenericManager.h"
#include <cstdio>

namespace Habanero
{

	template <typename T>
	class Specific_ptr
	{
		friend class GenericManager<T>;
	private:
		unsigned long id;

		T* object;
		static GenericManager<T> * manager;

	public:
		//TODO: Przemyśleć to...
		Specific_ptr()
		{
			this->object = manager->getErrorPointer().getObject();

			manager->registerSpecific_ptr(this);
		}

		Specific_ptr(T * object)
		{
			this->object = object;

			manager->registerSpecific_ptr(this);
		}

		Specific_ptr(const Specific_ptr<T> &ptr)
		{
			this->object = ptr.getObject();

			manager->registerSpecific_ptr(this);
		}

		~Specific_ptr()
		{
			manager->unregisterPtr(this);
		}

		inline unsigned long const & getId() const
		{
			return id;
		}

		inline T* getObject() const
		{
			return object;
		}


	private:

		void setId(unsigned long newId)
		{
			id = newId;
		}

	public:
		Specific_ptr<T> &operator=(const Specific_ptr<T> &ptr)
		{
			manager->unregisterPtr(this);
			this->object = ptr.getObject();
			manager->registerSpecific_ptr(this);
			return *this;
		}

		bool operator==(Specific_ptr<T> const &b) const
		{
			return id == b.id;
		}

		template <typename T1>
		bool operator==(Specific_ptr<T1> const &b) const
		{
			return false;
		}

		T* operator->() const
		{
			return object;
		}

/*
		static Specific_ptr<T> * __create(T* object)
		{
			return new Specific_ptr<T > (object);
		}
*/
	};

	template<typename T>
			GenericManager<T> * Specific_ptr<T>::manager = NULL;
}
