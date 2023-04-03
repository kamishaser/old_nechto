#pragma once
#include "entity.h"
#include "group.h"
#include "connecter.h"
#include "nodeStorage.h"

namespace nechto
{
	class garbageCollector
	{
	public:
		//вычисл€ет нижний уровень ноды. ≈сли ноды - мусор удал€ет и возвращает true. »наче false
		static bool updateNode(existing<nodePtr> node)
		{
			if (node.type() == nodeT::Deleted)
				return false;
			if (node.type() == nodeT::Hub)
				return false;
			ushort level = getNodeLevel(node);
			if (level == 0)
			{
				creator::deleteNode(node);
				return true;
			}
			node.node()->bottomLevel = level;
			return false;
		}
		ushort updateAllocator(ushort allocatorNumber, ushort beginNumber = 0, ushort endNumber = 65535)
		{
			ushort numberOfDeleted = 0;
			for (ushort i = beginNumber; i < endNumber; ++i)
			{
				if (updateNode(nodePtr(allocatorNumber, i)))
					++numberOfDeleted;
			}
		}
	private:
		static ushort getNodeLevel(existing<nodePtr> node)
		{
			if (entityPtr::match(node) && isStrongExternalConnection(node))
				return 1;
			ushort bottomLevel = 0;
			if (groupPtr::match(node))
			{
				groupPointer gi(node);
				do
				{
					ushort level = getConnectionLevel(gi);
					if (level > bottomLevel)
						bottomLevel = level;
				} while (gi.stepForward());
			}
			portPointer ptr(node);
			do
			{
				ushort level = getConnectionLevel(ptr);
				if (level > bottomLevel)
					bottomLevel = level;
			} while (ptr.stepForward());
			return bottomLevel;
		}
		static ushort getConnectionLevel(pointer ptr)
		{
			if (!isStrongConnection(ptr))
				return 0;
			ushort bottomLevel =  ptr.get().node()->bottomLevel + 1;
			//защита от замкнутых сильных св€зей с автоматим удалением оных
			if (bottomLevel == 255)
			{
				nearestDisconnect(ptr);
				return 0;
			}
			return bottomLevel;
		}
		static bool isStrongConnection(pointer p0)
		{
			if (!p0.get().exist())
				return false;
			pointer p1 = reversePort(p0);
			switch (p0.get().type())
			{
			case nodeT::Entity:
				return isEntityBackConnectionStrong(p1);
			case nodeT::Group:
				return isGroupBackConnectionStrong(p1);
			case nodeT::Struct:
				//return isStructBackConnectionStrong(node, ptr.getPurpose());
			default:
				return false;
			}
		}
		static bool isEntityBackConnectionStrong(pointer reverseConnection)
		{
			if(!isStrongExternalConnection(reverseConnection.getPurpose()))
				return false;
			if (reverseConnection.getHubNumber() == 0)
				return true;
			return false;
		}
		static bool isGroupBackConnectionStrong(pointer reverseConnection)
		{
			if (!groupPtr(reverseConnection.getPurpose()).isStrong())
				return false;
			nodePtr hub = reverseConnection.hub;
			if (hub.type() == nodeT::Hub && hub.subtype() == 1)
				return true;
			return false;
		}
		//bool isStructBackConnectionStrong(structPtr structure, existing<nodePtr> node);
		static bool isStrongExternalConnection(entityPtr node)
		{
			if (node.isOneSideLink())
				return false;
			if (!node.entityExist())
				return false;
			return true;
		}
	};
}