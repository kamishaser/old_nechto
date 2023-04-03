#pragma once
#include "entity.h"
#include <functional>
#include <set>

namespace nechto
{
	class verificator
	{
	public:
		static bool connectionCheck(pointer p1)
		{
			if (!p1.get().exist())
				return true;//соединения нет, тут всё хорошо
			pointer p2 = reversePort(p1);
			if((p1.get() != p2.getPurpose()) || (p1.getPurpose() != p2.get()))
				return false;
			if (p1 != reversePort(p2))
				return false;
			return true;
		}
		static bool checkHubChain(nodePtr node)
		{
			portPointer p1(node);
			if (!node.hub().exist())
				return true;
			nodePtr lastHub = node;
			std::set<nodePtr> repeatDefence;
			repeatDefence.insert(node);
			p1.goToNextHub();
			do
			{

				if (p1.hub.type() != nodeT::Hub)
					return false;
				if (hubPtr(p1.hub).hub() != lastHub)
					return false;
				if (hubPtr(p1.hub).number() != p1.getHubNumber())
					return false;
				if (p1.hub.subtype() != 0)
					return false;
				if (repeatDefence.contains(p1.hub))
					return false;
				repeatDefence.insert(p1.hub);
			} while (p1.goToNextHub());
			return true;
		}
		static bool checkGroupHubChain(groupPtr group)
		{
			groupPointer g1(group);
			nodePtr lastHub = g1.hub;
			std::set<nodePtr> repeatDefence;
			while (true)
			{

				if (g1.hub.type() != nodeT::Hub)
					return false;
				if (hubPtr(g1.hub).number() != g1.getHubNumber())
					return false;
				if (g1.hub.subtype() != 1)
					return false;
				if (repeatDefence.contains(g1.hub))
					return false;
				repeatDefence.insert(g1.hub);
				if (!g1.goToNextHub())
					return true;
				if (hubPtr(g1.hub).hub() != lastHub)
					return false;
				lastHub = g1.hub;
			}
		};
		static bool checkPointer(pointer p0)
		{
			if (p0.hub == p0.getPurpose())
				return p0.getHubNumber() == 0;

			hubPtr hub = p0.hub;
			if (hub.inGroup())
			{
				if (p0.getPurpose().type() != nodeT::Group)
					return false;
				groupPointer gi(p0.getPurpose());
				for (ui32 i = 0; i < hub.number(); ++i)
					gi.goToNextHub();
				assert(gi.getHubNumber() == hub.number());//убрать после первого использования (туплю)
				if (gi.hub != hub)
					return false;
				if (hubPtr(p0.hub).number() != gi.getHubNumber())
					return false;
			}
			else
			{
				portPointer p1(p0.getPurpose());
				for (ui32 i = 0; i < hub.number(); ++i)
					p1.goToNextHub();
				assert(p1.getHubNumber() == hub.number());//убрать после первого использования (туплю)
				if (p1.hub != hub)
					return false;
				if (hubPtr(p0.hub).number() != p1.getHubNumber())
					return false;
			}
			return true;
		}
	};
}