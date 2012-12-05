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

#include <map>
#include <set>

/*
 * Założenia:
 * 
 * Klasa dziedzicząca po GenericManager musi:
 * 
 * 1) zainicjować singleInstance za pomocą metody setGlobalManagerInstance(..).
 * 2) zainicjować errorPointer za pomocą metody setGlobalErrorPointer(..)
 * 
 * Destruktor GenericManager zwalnia errorPointer'a.
 */

namespace Habanero
{

	template <typename T>
	class Specific_ptr;

	template <typename T>
	class GenericManager
	{
		friend class Specific_ptr<T>;

	protected:

		class MapRecord
		{
		private:
			T * object;
			std::set< Specific_ptr<T> * > refs;
		public:
			
			MapRecord(T* object) : object(object)
			{
			}

			void remove()
			{
				HASSERT(getNumberOfReferences() == 0);
				if (object != NULL)
					delete object;
			}

			int getNumberOfReferences() const
			{
				return refs.size();
			}

			void addNewReference(Specific_ptr<T> * p)
			{
				refs.insert(p);
			}

			void removeReference(Specific_ptr<T> * p)
			{
				refs.erase(p);
			}
			
			T* getObject() const
			{
				return object;
			}
		};

		static GenericManager<T> * singleInstance;
		static Specific_ptr<T> * errorPointer;
		std::map<unsigned long int, MapRecord > records;
		std::map<T*, unsigned long int> ptrToId;

		static unsigned long int freeid;

		GenericManager(GenericManager<T> *manager)
		{
			singleInstance = manager;
			freeid = 1;
		}
		
		virtual ~GenericManager()
		{
			delete errorPointer;
			typename std::map<unsigned long int, MapRecord >::iterator it;
			for (it = records.begin(); it != records.end(); ++it)
				it->second.remove();
			records.clear();
			ptrToId.clear();
		}

	public:
		static GenericManager<T> &getInstance()
		{
			return *singleInstance;
		}

		static GenericManager<T> *getInstancePtr()
		{
			return singleInstance;
		}
		
	protected:
		
		static void setGlobalManagerInstance(GenericManager<T> * newThis)
		{
			HASSERT(Specific_ptr<T>::manager == NULL);
			Specific_ptr<T>::manager = newThis;
		}
		
		static void setGlobalErrorPointer(Specific_ptr<T> * newErrorPointer)
		{
			errorPointer = newErrorPointer;
		}
		
		unsigned long int getNextFreeID()
		{
			return freeid++;
		}

		Specific_ptr<T> createSpecific_ptr(T* object)
		{
			/*
			 * TODO: tu można dodać test, czy wskaźnik nie jest już wykorzystywany
			 */
			return Specific_ptr<T>(object);
		}
		
		/*
		 * p niekoniecznie ma prawidłowe ID, ale ma prawidłowy object
		 */
		void registerSpecific_ptr(Specific_ptr<T> * p)
		{
			typename std::map<T*, unsigned long int>::iterator itId = ptrToId.find(p->getObject());
			typename std::map<unsigned long int, MapRecord >::iterator it;
			
			if(itId == ptrToId.end())
			{
				unsigned long int newId = getNextFreeID();
				it = records.insert(std::make_pair(newId, MapRecord(p->getObject()))).first;
				ptrToId.insert(std::make_pair(p->getObject(), newId));
				p->setId(newId);
				it->second.addNewReference(p);
			}
			else
			{
				it = records.find(itId->second);
				p->id = itId->second;	
				it->second.addNewReference(p);
			}
		}

		void unregisterPtr(Specific_ptr<T> * p)
		{
			typename std::map<unsigned long int, GenericManager<T>::MapRecord >::iterator it = records.find(p->getId());
			HASSERT(it != records.end());
			it->second.removeReference(p);

			// Nie będziemy usuwać tych obiektów. Co najwyżej wywołamy unloada na nich...
			/*if (it->second.getNumberOfReferences() == 0)
			{
				typename std::map<T*, unsigned long int >::iterator itId = ptrToId.find(p->getObject());
				HASSERT(itId != ptrToId.end());
				ptrToId.erase(itId);

				//Na razie tak, żeby nie było memory leaków.
				//delete p->getObject();
				//TODO: kolejkowanie usuwania?
			}*/
		}


		Specific_ptr<T> getPtrFromId(unsigned long int id)
		{
			typename std::map<unsigned long int, MapRecord>::iterator it = records.find(id);
			if(it == records.end())
				return *errorPointer;
			/*
			 * TODO: optymalizacja
			 */
			return Specific_ptr<T> (it->second.getObject());
		}
		

		// Nie przetestowane...
		void updateIdWithNewAddress(unsigned long int id, T* newAddress)
		{
			typename std::map<unsigned long int, MapRecord >::iterator recIt;
			
			recIt = records.find(id);
			HASSERT(recIt != records.end());
			
			// 1) usuwanie starego adresu
			typename std::map<T*, unsigned long int>::iterator oldPtrToIdIt = ptrToId.find(recIt->second.object);
			HASSERT(oldPtrToIdIt != ptrToId.end());
			ptrToId.erase(oldPtrToIdIt);
			
			// 2) wstawianie nowego adresu
			recIt->second.object = newAddress;
			ptrToId.insert(make_pair(newAddress, id));
			
			// 3) aktualizacja ptr'ow
			typename std::set< Specific_ptr<T> * >::iterator refIt;
			
			for(refIt = recIt->second.refs.begin(); refIt != recIt->second.refs.end(); refIt++)
			{
				refIt->object = newAddress;
			}
		}
		
		public:
			inline Specific_ptr<T> getErrorPointer()
			{
				return *errorPointer;
			}
	};

	template<typename T>
			ulong GenericManager<T>::freeid = 1;

	template<typename T>
			GenericManager<T> *GenericManager<T>::singleInstance = NULL;
	
	template<typename T>
			Specific_ptr<T> * GenericManager<T>::errorPointer = NULL;
}
