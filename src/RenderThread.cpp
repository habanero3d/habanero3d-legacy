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


#include "Node.h"
#include "RenderThread.h"

/* ten kod został wyłączony w całości, ponieważ nie realizował żadnej funkcji
 * w wydaniu windowsowym. Zostaje, bo kiedyś ma zacząć działać.
 *
 * akuda
 */


//TODO: zakladam, ze delete wezel = delete poddrzewo
//Ten kod NIE dziala, trzeba najpierw zdefiniowac semantyke setNextScene



namespace Habanero
{
/*	static void renderScene(Node *root)
	{
	}

	RenderThread::RenderThread()
	{
		stopped = false;
	}

	void RenderThread::setNextScene(Node *root)
	{
		lock.acquire();
		delete pendingScene;
		pendingScene = root;
		lock.release();
		ready.signal();
	}

	void RenderThread::stop()
	{
		stopped = true;
	}

	void RenderThread::main()
	{
		while (!stopped)
		{
			ready.wait();
			Node *root = pendingScene;
			delete pendingScene;
			pendingScene = null;
			renderScene(pendingScene);
		}
		delete pendingScene;
	}
 *
 * */
}