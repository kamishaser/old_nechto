#pragma once
#include "object.h"
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
			if (nonTypedObjectPtr::match(node) && isStrongExternalConnection(node))
				return 1;
			ushort bottomLevel = 0;
			if (groupPtr::match(node))
			{
				groupIterator gi(node);
				do
				{
					ushort level = getConnectionLevel(gi);
					if (level > bottomLevel)
						bottomLevel = level;
				} while (gi.stepForward());
			}
			portIterator iter(node);
			do
			{
				ushort level = getConnectionLevel(iter);
				if (level > bottomLevel)
					bottomLevel = level;
			} while (iter.stepForward());
			return bottomLevel;
		}
		static const ushort maxConnectionLevel = 1024;
		static ushort getConnectionLevel(iterator iter)
		{
			if (!isStrongConnection(iter))
				return 0;
			ushort bottomLevel =  iter.get().node()->bottomLevel + 1;
			//защита от замкнутых сильных св€зей с автоматим удалением оных
			if (bottomLevel > maxConnectionLevel)
			{
				nearestDisconnect(iter);
				return 0;
			}
			return bottomLevel;
		}
		static bool isStrongConnection(iterator iter)
		{
			nodePtr node = iter.get();
			if (!node.exist())
				return false;
			switch (node.type())
			{
			case nodeT::Object:
				return isObjectBackConnectionStrong(node, iter.getPurpose());
			case nodeT::Group:
				return isGroupBackConnectionStrong(node, iter.getPurpose());
			case nodeT::Struct:
				//return isStructBackConnectionStrong(node, iter.getPurpose());
			default:
				return false;
			}
		}
		static bool isObjectBackConnectionStrong(nonTypedObjectPtr object, existing<nodePtr> node)
		{
			if(!isStrongExternalConnection(object))
				return false;
			for (int i = 0; i < 4; ++i)
				if (object.connection(i) == node)
					return true;
			return false;
		}
		static bool isGroupBackConnectionStrong(groupPtr group, existing<nodePtr> node)
		{
			if (!group.isStrong())
				return false;
			groupIterator gi(group);
			do
			{
				if (gi.get() == node)
					return true;
			} while (gi.stepForward());
			return false;
		}
		//bool isStructBackConnectionStrong(structPtr structure, existing<nodePtr> node);
		static bool isStrongExternalConnection(nonTypedObjectPtr object)
		{
			if (object.isUniqueOwner())
				return false;
			if (!object.dataExist())
				return false;
			return true;
		}
	};
}