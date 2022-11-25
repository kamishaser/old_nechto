#pragma once
#include "typeNames.h"
#include "text.h"
#include "nodeTypeProperties.h"
#include "connectionIterator.h"

namespace nechto
{
	const std::u16string to_string(nodePtr address)
	{
		return static_cast<std::u16string>(u"[") + 
			std::to_u16string((int)address.getFirst()) + L' ' + 
			std::to_u16string((int)address.getSecond()) + L']';
	}
	const std::u16string nodeType(nodePtr address)
	{
		return typeName::nodeT[address->getType()];
	}
	const std::u16string nodeSubtype(nodePtr address)
	{
		switch (address->getType())
		{
		case node::Variable:
			return typeName::variableT[address->getSubtype()];
		case node::MathOperator:
			return typeName::mathOperatorShortT[address->getSubtype()];
		case node::Pointer:
			return typeName::pointerT[address->getSubtype()];
		default:
			return u"";
		}
	}
	const std::u16string nodeData(nodePtr v1)
	{
		if (v1 == nullNodePtr)
			return u"error";
		i64 data = v1->getData<i64>();
		void* temptr = &data;
		switch (v1->getType())
		{
		case node::Variable:
			if(v1->getSubtype())
				return std::to_u16string(*static_cast<i64*>(temptr));
			else
				return std::to_u16string(*static_cast<f64*>(temptr));
		case node::Text:
			return text::get(v1);
		case node::Method:
			return method::getMethodName(v1);
		case node::ExternalObject:
			if (v1->getData<externalObject*>())
				return v1->getData<externalObject*>()->getTypeName();
		}
		return u"";
	}

	std::u16string nodeProperties(const nodePtr v1)
	{
		if (!v1.exist())
			return u"nullNodePtr";
		return std::u16string(to_string(v1) + L' ' + nodeType(v1) + L' ' + nodeSubtype(v1) + L' ' + nodeData(v1));
	}
	std::u16string numConnectionsList(const nodePtr v1)
	{
		assert(v1.exist());
		std::u16string temp = nodeProperties(v1) + u"________________________________\n";
		for (int i = 0; i < 4; ++i)
			temp += u"   " + nodeProperties(v1->connection[i].load()) + L'\n';
		return temp;
	}
	std::u16string connectionsList(const nodePtr v1)
	{
		std::u16string temp = nodeProperties(v1) + L'\n';
		if (!v1.exist())
			return temp;
		if (typeCompare(v1, node::Group))
		{
			groupIterator gi(v1);
			do
			{
				print(numConnectionsList(gi.currentHub));
				temp += numConnectionsList(gi.currentHub) + u"\n";
			} while (gi.GoToNextHub());
			temp += u"\n mainChain:\n";
		}
		connectionIterator i(v1);
		do {
			temp += numConnectionsList(i.currentHub) + u"\n";
		} while (i.GoToNextHub());
		return temp;
	}
}