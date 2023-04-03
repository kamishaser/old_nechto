#pragma once
#include "hubManager.h"
#include <map>


namespace nechto
{
	class compressor
	{
	public:
		//сжать основную цепочку с перемещенем итераторов
		static void compress_W_MI(const portPointer& begin)
		{
			portPointer pullPointer = begin;
			portPointer pushPointer = begin;
			std::map<ui32, nodePtr> ptrSet;
			fillPortPointerSet(ptrSet, begin.getPurpose());
			do
			{
				if (pullPointer.get().exist())
				{
					connecter::swap(pullPointer, pushPointer);
					ui32 pos = pullPointer.getGlobalPos();
					if (ptrSet.contains(pullPointer.getGlobalPos()))
						portPointerPtr(ptrSet[pos]).setHPPair(pushPointer.getHPPair());
					pushPointer.stepForward();
				}
			} while (pullPointer.stepForward());
			if (pushPointer.getLocalPos() != 0)
				pushPointer.goToNextHub();
			hubManager::eraseHubWithNoNotificationPointers(pushPointer, -1);
		}
		//сжать группу с перемещенем итераторов
		static void compress_W_MI(const groupPointer& begin)
		{
			groupPointer pullPointer = begin;
			groupPointer pushPointer = begin;
			std::map<ui32, nodePtr> ptrSet;
			fillGroupPointerSet(ptrSet, begin.getPurpose());
			do
			{
				if (pullPointer.get().exist())
				{
					connecter::swap(pullPointer, pushPointer);
					ui32 pos = pullPointer.getGlobalPos();
					if (ptrSet.contains(pullPointer.getGlobalPos()))
						groupPointerPtr(ptrSet[pos]).setHPPair(pushPointer.getHPPair());
					pushPointer.stepForward();
				}
			} while (pullPointer.stepForward());
			if (pushPointer.getLocalPos() != 0)
				pushPointer.goToNextHub();
			hubManager::eraseHubWithNoNotificationPointers(pushPointer, -1);
		}
		///////////////////////////////////////////////////////////////////
		//сжать основную цепочку без перемещения итераторов
		static void compress_WO_MI(const portPointer& begin)
		{
			portPointer pullPointer = begin;
			portPointer pushPointer = begin;
			do
			{
				if (pullPointer.get().exist())
				{
					connecter::swap(pullPointer, pushPointer);
					pushPointer.stepForward();
				}
			} while (pullPointer.stepForward());
			if (pushPointer.getLocalPos() != 0)
				pushPointer.goToNextHub();
			hubManager::eraseHub(pushPointer, -1);
		}
		//сжать группу без перемещения итераторов
		static void compress_WO_MI(const groupPointer& begin)
		{
			groupPointer pullPointer = begin;
			groupPointer pushPointer = begin;
			do
			{
				if (pullPointer.get().exist())
				{
					connecter::swap(pullPointer, pushPointer);
					pushPointer.stepForward();
				}
			} while (pullPointer.stepForward());
			if (pushPointer.getLocalPos() != 0)
				pushPointer.goToNextHub();
			hubManager::eraseHub(pushPointer, -1);
		}






		static void fillPortPointerSet(std::map<ui32, nodePtr>& ptrSet, nodePtr node)
		{
			portPointer ptr(node);
			do
			{
				if (isPortPointerPtr(ptr))
				{
					portPointerPtr temp(ptr.get());
					ptrSet.emplace(temp.getGlobalPos(), temp);
				}
			} while (ptr.stepForward());
		}
		static void fillGroupPointerSet(std::map<ui32, nodePtr>& ptrSet, nodePtr node)
		{
			portPointer ptr(node);
			do
			{
				if (isGroupPointerPtr(ptr))
				{
					groupPointerPtr temp(ptr.get());
					ptrSet.emplace(temp.getGlobalPos(), temp);
				}
			} while (ptr.stepForward());
		}
	private:
		static bool isPortPointerPtr(portPointer ptr)
		{
			nodePtr temp = ptr.get();
			if (portPointerPtr::match(temp))
				if (temp.connection(0) == ptr.getPurpose())
					return true;
			return false;
		}
		static bool isGroupPointerPtr(portPointer ptr)
		{
			nodePtr temp = ptr.get();
			if (groupPointerPtr::match(temp))
				if (temp.connection(0) == ptr.getPurpose())
					return true;
			return false;
		}
	};
}