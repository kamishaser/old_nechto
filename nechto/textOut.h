#pragma once
#include "typeNames.h"
#include "text.h"
#include "connectionIterator.h"
#include "string_numberConverter.h"
#include "variablePtr.h"

namespace nechto
{
	const std::u16string to_string(nodePtr address)
	{
		return static_cast<std::u16string>(u"[") + 
			snConverter::to_ustr((int)address.getFirst()) + u' ' + 
			snConverter::to_ustr((int)address.getSecond()) + u']';
	}
	const std::u16string to_string(hubPosPair hpp)
	{
		return ustr(u"{") + to_string(hpp.hub) + ustr(u" ") +
			snConverter::to_ustr((ui32)hpp.getGlobalPos()) + ustr(u" ") +
			((hpp.hub.exist()) ? to_string(hpp.follow()) : ustr(u"___")) + ustr(u"}");
	}
	ustr getUstrObjectData(nonTypedObjectPtr objectNode); //реализация в namedExCon
	const std::u16string nodeData(nodePtr v1)
	{
		if (v1 == nullptr)
			return u"nullptr";
		switch (v1.type())
		{
		case nodeT::Variable:
			if (v1.subtype())
				return snConverter::to_ustr(i64VariablePtr(v1).get());
			else
				return snConverter::to_ustr(f64VariablePtr(v1).get());
		case nodeT::Pointer:
			if (v1.subtype() != pointerT::Simple)
				return to_string(v1.connection(0));
			else
				return to_string(pointerPtr(v1).getHPPair());
		case nodeT::Text:
			if (textPtr(v1).textExist())
				return *textPtr(v1);
			else
				return u"nullTextPtr";
		case nodeT::Object:
			return getUstrObjectData(v1);
		default:
			return u"";
		}
	}

	std::u16string nodeProperties(nodePtr v1)
	{
		if (!v1.exist())
			return u"nullNodePtr";
		return ustr(to_string(v1) + u' ' + 
			typeName::getTypeAndSubtypeName(v1.type(), v1.subtype()) + u' ' + 
			nodeData(v1));
	}
	std::u16string numConnectionsList(nodePtr v1)
	{
		assert(v1.exist());
		std::u16string temp = nodeProperties(v1) + u"____________________________\n";
		for (int i = 0; i < 4; ++i)
			temp += u"   " + nodeProperties(v1.connection(0)) + u'\n';
		return temp;
	}
	std::u16string connectionsList(nodePtr v1)
	{
		ustr temp = nodeProperties(v1) + u'\n';
		if (!v1.exist())
			return temp;
		if (v1.type() == nodeT::Group)
		{
			groupPtr group(v1);
			groupIterator iter(group);
			do
			{
				//print(numConnectionsList(iter.hub));
				temp += numConnectionsList(iter.hub) + u"\n";
			} while (iter.goToNextHub());
			temp += u"\n mainChain:\n";
		}
		portIterator iter(v1);
		do {
			temp += numConnectionsList(iter.hub) + u"\n";
		} while (iter.goToNextHub());
		return temp;
	}
}