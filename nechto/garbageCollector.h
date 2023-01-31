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
			if (nonTypedEntityPtr::match(node) && isStrongExternalConnection(node))
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
		static const ushort maxConnectionLevel = 1024;
		static ushort getConnectionLevel(pointer ptr)
		{
			if (!isStrongConnection(ptr))
				return 0;
			ushort bottomLevel =  ptr.get().node()->bottomLevel + 1;
			//защита от замкнутых сильных св€зей с автоматим удалением оных
			if (bottomLevel > maxConnectionLevel)
			{
				nearestDisconnect(ptr);
				return 0;
			}
			return bottomLevel;
		}
		static bool isStrongConnection(pointer ptr)
		{
			nodePtr node = ptr.get();
			if (!node.exist())
				return false;
			switch (node.type())
			{
			case nodeT::Entity:
				return isEntityBackConnectionStrong(node, ptr.getPurpose());
			case nodeT::Group:
				return isGroupBackConnectionStrong(node, ptr.getPurpose());
			case nodeT::Struct:
				//return isStructBackConnectionStrong(node, ptr.getPurpose());
			default:
				return false;
			}
		}
		static bool isEntityBackConnectionStrong(nonTypedEntityPtr entity, existing<nodePtr> node)
		{
			if(!isStrongExternalConnection(entity))
				return false;
			for (int i = 0; i < 4; ++i)
				if (entity.connection(i) == node)
					return true;
			return false;
		}
		static bool isGroupBackConnectionStrong(groupPtr group, existing<nodePtr> node)
		{
			if (!group.isStrong())
				return false;
			groupPointer gi(group);
			do
			{
				if (gi.get() == node)
					return true;
			} while (gi.stepForward());
			return false;
		}
		//bool isStructBackConnectionStrong(structPtr structure, existing<nodePtr> node);
		static bool isStrongExternalConnection(nonTypedEntityPtr entity)
		{
			if (entity.isUniqueOwner())
				return false;
			if (!entity.dataExist())
				return false;
			return true;
		}
	};
}