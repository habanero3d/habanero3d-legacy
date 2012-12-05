/*
Copyright 2009-2012 Andrzej Skalski, Piotr Kufel, Piotr Bia�ecki, Micha� �ochowski, and Micha� Szczepaniak
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


/* @file
@author: Bialy
*/
#include <set>
#include <tuple>
#include <vector>

#include "matrix4.h"
#include "vector3.h"
#include "VSplit.h"
#include "Memory.h"
#include "VDLODSkinnedMesh.h"
#include "IBoundingVolume.h"
#include "VDLODSMF.h"
#include "Frustum.h"

namespace Habanero
{
	VDLODSkinnedMesh::VDLODSkinnedMesh(const std::string &filename,
			Resource::Mapping &materials,
			Resource::Mapping &textures,
			Resource::Mapping &skeletons,
			Resource::Mapping &joints,
			Resource::Mapping &animations)
		: IResource(), filename(filename), materials(materials), textures(textures), skeletons(skeletons)
	{
		setLoadState(Discovered);
		geometry = NULL;
		boundingVolume = NULL;
	}

	VDLODSkinnedMesh::~VDLODSkinnedMesh()
	{
		unload();
	}

	uint VDLODSkinnedMesh::getNumVertexSplits() const
	{
		return numVertexSplits;
	}

	// helper, copypasta from sphere::planeInterference
	bool VDLODSkinnedMesh::isOverOrThrough(const plane<float>& plane, const vector3f &center, float radius)
	{
		float dist = dot(plane.normal, center) + plane.d;	// signed distance from plane
		if(dist > 0)	// the center lies on the same side as the plane normal points to <=> sphere is over the plane
			return true;
		else			// center lies on different side of the plane
			if(-dist > radius)	// radius is too small for the sphere to go through a plane
				return false;
			else
				return true;
		return true;
	}

	GenericGeometry<SkinVertex> * VDLODSkinnedMesh::getGeometry(const Frustum<float>& ofru, const matrix4f &parentToLocal)
	{
		Frustum<float> fru(ofru);
		fru.transform(parentToLocal);

		// overwrite previous changes
		std::vector<uint> counters(geometry->getNumSubMeshes());
		for(int i = 0; i < geometry->getNumSubMeshes(); i++)
		{
			ExtendableSubMesh * extSubMesh = (ExtendableSubMesh *)(tmpGeometry->getSubMesh(i));
			const SubMesh * sm = geometry->getSubMesh(i);
			counters[i] = sm->getNumIndices();
			std::copy(sm->getIndices(), sm->getIndices() + sm->getNumIndices(), extSubMesh->getIndices());
			extSubMesh->setNumIndices(sm->getNumIndices());
		}

		std::set<uint> availableRefinementsSet(activeVSplits.begin(), activeVSplits.end());
		std::set<uint> performedRefinementsSet;
		std::set<uint> notPerformedRefinementsSet;
		
		do
		{
			for(std::set<uint>::iterator it = availableRefinementsSet.begin(); it != availableRefinementsSet.end(); ++it)
			{
				uint activeSplitNum = *it;
				const VSplit &split = vertexSplits[activeSplitNum];
				const vector3f &referencePos = vsplitRefinements[activeSplitNum].referencePosition;
				const vector3f &referenceNormal = vsplitRefinements[activeSplitNum].referenceNormal;
				float radius = vsplitRefinements[activeSplitNum].radius;

				bool perform = true;

				const std::vector<plane<float>>& planes = fru.getPlanes();
				for(uint i = 0 ; i < planes.size(); ++i)
					if(!isOverOrThrough(planes[i], referencePos, radius))
						perform = false;
				
				const vector3f tmpVec = referencePos - fru.viewPoint;
				const float tmpLength = tmpVec.length();
				const float tmpDot = dot(tmpVec, referenceNormal);

				if((tmpDot > 0) && (tmpDot * tmpDot > tmpLength * tmpLength * vsplitRefinements[activeSplitNum].sinAlphaSquared))
					perform = false;

				if(perform)
				{
					for(int j = 0; j < split.numAdditions; j++)
					{
						MF2::TriangleAddition& addition = split.getTriangleAdditions()[j];
						for(int k = 0; k < 3; k++)
						{
							ExtendableSubMesh * sm = (ExtendableSubMesh *)(tmpGeometry->getSubMesh(split.subMeshId));
							sm->setIndex(counters[split.subMeshId] + k, addition.indices[k]);
						}
						counters[split.subMeshId] +=3;
					}

					ExtendableSubMesh * sm = (ExtendableSubMesh *)(tmpGeometry->getSubMesh(split.subMeshId));
					sm->setNumIndices(counters[split.subMeshId]);

					for(int j = 0; j < split.numMoves; j++)
					{
						bool performed = false;
						MF2::VertexMove& move = split.getVertexMoves()[j];
						for(int ind = 0; ind < sm->getNumIndices(); ind += 3)
						{
							if(move.pattern[0] != sm->getIndex(ind)
							|| move.pattern[1] != sm->getIndex(ind + 1)
							|| move.pattern[2] != sm->getIndex(ind + 2))
								continue;

							for(int offs = 0; offs < 3; offs++)
								if(move.fromId == sm->getIndex(ind + offs))
								{
									sm->setIndex(ind + offs, move.toId);
									performed = true;
								}

								break;	// pattern was matched, it can be performed only once (there will be no exact triangles in mesh), no need to check more
						}

						HASSERT(performed);
					}

					performedRefinementsSet.insert(activeSplitNum);
				}	// if(perform)
				else
					notPerformedRefinementsSet.insert(activeSplitNum);
			}

			for(std::set<uint>::iterator it = performedRefinementsSet.begin(); it != performedRefinementsSet.end(); ++it)
			{
				std::set<uint>::iterator p = availableRefinementsSet.find(*it);
				if(p != availableRefinementsSet.end())
					availableRefinementsSet.erase(p);
			}

			for(std::set<uint>::iterator it = notPerformedRefinementsSet.begin(); it != notPerformedRefinementsSet.end(); ++it)
			{
				std::set<uint>::iterator p = availableRefinementsSet.find(*it);
				if(p != availableRefinementsSet.end())
					availableRefinementsSet.erase(p);
			}

			for(uint vsplitId = 0; vsplitId < numVertexSplits; vsplitId++)
			{
				std::set<uint>::iterator p = performedRefinementsSet.find(vsplitId);
				std::set<uint>::iterator p2 = notPerformedRefinementsSet.find(vsplitId);
				if(p == performedRefinementsSet.end() && p2 == notPerformedRefinementsSet.end())	// it was not checked syet, so potentially it can be performed
				{
					bool parentsPerformed = true;
					for(uint parents = 0; parents < vsplitParents[vsplitId].size(); parents++)
						if(performedRefinementsSet.find(vsplitParents[vsplitId][parents]) == performedRefinementsSet.end())
							parentsPerformed = false;
					if(parentsPerformed)
						availableRefinementsSet.insert(vsplitId);
				}
			}
		}
		while(!availableRefinementsSet.empty());
		
		return tmpGeometry;
	}

	bool VDLODSkinnedMesh::load()
	{
		if (getLoadState() != Discovered)
			return false;
		InitInfo initInfo = VDLODSMF::loadFromFile(filename.c_str(), materials, textures, skeletons);
		boundingVolume = initInfo.boundingVolume;
		skeleton = initInfo.skeleton;

		geometry = new GenericGeometry<SkinVertex>(initInfo.geometry, true);
		tmpGeometry = new GenericGeometry<SkinVertex>(initInfo.geometry);

		numVertexSplits = initInfo.vertexSplits.size();
		vertexSplits = uninitializedAlloc<VSplit>(initInfo.vertexSplits.size());
		for(int i = 0; i < initInfo.vertexSplits.size(); i++)
			new(&vertexSplits[i]) VSplit(initInfo.vertexSplits[i]);

		vsplitRefinements = new MF2::VSplitRefinement[numVertexSplits];
		std::copy(initInfo.refinements.begin(), initInfo.refinements.end(), vsplitRefinements);
		// hierarchia vertex�w
		vsplitChildren.resize(numVertexSplits);
		vsplitParents.resize(numVertexSplits);

		for(uint i = 0; i < numVertexSplits; i++)
		{
			for(uint j = 0; j < initInfo.vertexChildren[i].size(); j++)
			{
				uint child = initInfo.vertexChildren[i][j];
				vsplitChildren[i].push_back(child);
				vsplitParents[child].push_back(i);
			}
		}

		for(uint i = 0; i < numVertexSplits; i++)
			if(vsplitParents[i].empty())
				activeVSplits.push_back(i);

		setLoadState(Loaded);
		return true;
	}
	
	bool VDLODSkinnedMesh::unload()
	{
		if(getLoadState() == Discovered)
			return false;

		delete [] vsplitRefinements;

		delete geometry;
		delete tmpGeometry;

		delete boundingVolume;
		uninitializedDelete(vertexSplits, numVertexSplits);
		setLoadState(Discovered);
		return true;
	}
}