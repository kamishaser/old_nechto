#pragma once
#include "hubManager.h"
#include <map>


namespace nechto
{
	class compressor
	{
	public:
		//сжать основную цепочку с перемещенем итераторов
		static void compress_W_MI(const portIterator& begin)
		{
			portIterator pullIter = begin;
			portIterator pushIter = begin;
			std::map<ui32, portIteratorPtr> iterSet;
			fillIteratorSet(iterSet, begin.getPurpose());
			do
			{
				if (pullIter.get().exist())
				{
					connecter::swap(pullIter, pushIter);
					ui32 pos = pullIter.getGlobalPos();
					if (iterSet.contains(pullIter.getGlobalPos()))
						iterSet[pos].setHPPair(pushIter.getHPPair());
					pushIter.stepForward();
				}
			} while (pullIter.stepForward());
			if (pushIter.getLocalPos() != 0)
				pushIter.goToNextHub();
			hubManager::eraseHubWithNoNotificationIterators(pushIter, -1);
		}
		//сжать группу с перемещенем итераторов
		static void compress_W_MI(const groupIterator& begin)
		{
			groupIterator pullIter = begin;
			groupIterator pushIter = begin;
			std::map<ui32, groupIteratorPtr> iterSet;
			fillIteratorSet(iterSet, begin.getPurpose());
			do
			{
				if (pullIter.get().exist())
				{
					connecter::swap(pullIter, pushIter);
					ui32 pos = pullIter.getGlobalPos();
					if (iterSet.contains(pullIter.getGlobalPos()))
						iterSet[pos].setHPPair(pushIter.getHPPair());
					pushIter.stepForward();
				}
			} while (pullIter.stepForward());
			if (pushIter.getLocalPos() != 0)
				pushIter.goToNextHub();
			hubManager::eraseHubWithNoNotificationIterators(pushIter, -1);
		}
		///////////////////////////////////////////////////////////////////
		//сжать основную цепочку без перемещения итераторов
		static void compress_WO_MI(const portIterator& begin)
		{
			portIterator pullIter = begin;
			portIterator pushIter = begin;
			do
			{
				if (pullIter.get().exist())
				{
					connecter::swap(pullIter, pushIter);
					pushIter.stepForward();
				}
			} while (pullIter.stepForward());
			if (pushIter.getLocalPos() != 0)
				pushIter.goToNextHub();
			hubManager::eraseHub(pushIter, -1);
		}
		//сжать группу без перемещения итераторов
		static void compress_WO_MI(const groupIterator& begin)
		{
			groupIterator pullIter = begin;
			groupIterator pushIter = begin;
			do
			{
				if (pullIter.get().exist())
				{
					connecter::swap(pullIter, pushIter);
					pushIter.stepForward();
				}
			} while (pullIter.stepForward());
			if (pushIter.getLocalPos() != 0)
				pushIter.goToNextHub();
			hubManager::eraseHub(pushIter, -1);
		}






		static void fillIteratorSet(std::map<ui32, portIteratorPtr>& iterSet, nodePtr node)
		{
			portIterator iter(node);
			do
			{
				if (isPortIteratorPtr(iter))
				{
					portIteratorPtr temp(iter.get());
					iterSet.emplace(temp.getGlobalPos(), temp);
				}
			} while (iter.stepForward());
		}
		static void fillIteratorSet(std::map<ui32, groupIteratorPtr>& iterSet, nodePtr node)
		{
			portIterator iter(node);
			do
			{
				if (isGroupIteratorPtr(iter))
				{
					groupIteratorPtr temp(iter.get());
					iterSet.emplace(temp.getGlobalPos(), temp);
				}
			} while (iter.stepForward());
		}
	private:
		static bool isPortIteratorPtr(portIterator iter)
		{
			nodePtr temp = iter.get();
			if (portIteratorPtr::match(temp))
				if (temp.connection(0) == iter.getPurpose())
					return true;
			return false;
		}
		static bool isGroupIteratorPtr(portIterator iter)
		{
			nodePtr temp = iter.get();
			if (groupIteratorPtr::match(temp))
				if (temp.connection(0) == iter.getPurpose())
					return true;
			return false;
		}
	};
}