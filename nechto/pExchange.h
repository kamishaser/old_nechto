#pragma once
#include "path.h"
#include "variablePtr.h"
#include "text.h"
#include "pointerPtr.h"
#include "connectionPointer.h"

namespace nechto
{
	bool operator>>(nodePtr node, i64& value)
	{
		if (!variablePtr::match(node))
			return false;
		value = variablePtr(node);
		return true;
	}
	bool operator>>(nodePtr node, f64& value)
	{
		if (!variablePtr::match(node))
			return false;
		value = variablePtr(node);
		return true;
	}
	bool operator>>(nodePtr node, ui32& value)
	{
		if (!variablePtr::match(node))
			return false;
		value = (i64)variablePtr(node);
		return true;
	}
	bool operator>>(nodePtr node, bool& value)
	{
		if (!variablePtr::match(node))
			return false;
		value = (i64)variablePtr(node);
		return true;
	}
	bool operator>>(nodePtr node, float& value)
	{
		if (!variablePtr::match(node))
			return false;
		value = (double)variablePtr(node);
		return true;
	}
	bool operator>>(nodePtr node, std::wstring& text)
	{
		if (!textPtr::match(node))
			return false;
		text = textPtr(node);
		return true;
	}
	bool operator>>(nodePtr node, nodePtr& node2)
	{
		if (!node.exist())
			return false;
		node2 = node;
		return true;
	}
	bool operator<<(nodePtr node, const f64 value)
	{
		if (!variablePtr::match(node))
			return false;
		variablePtr temp(node);
		temp = value;
		return true;
	}
	bool operator<<(nodePtr node, const i64 value)
	{
		if (!variablePtr::match(node))
			return false;
		variablePtr temp(node);
		temp = value;
		return true;
	}
	bool operator<<(nodePtr node, const float value)
	{
		if (!variablePtr::match(node))
			return false;
		variablePtr temp(node);
		temp = value;
		return true;
	}
	bool operator<<(nodePtr node, const ui32 value)
	{
		if (!variablePtr::match(node))
			return false;
		variablePtr temp(node);
		temp = i64(value);
		return true;
	}
	bool operator<<(nodePtr node, const bool value)
	{
		if (!variablePtr::match(node))
			return false;
		variablePtr temp(node);
		temp = i64(value);
		return true;
	}
	bool operator<<(nodePtr node, const std::wstring& text)
	{
		if (!textPtr::match(node))
			return false;
		textPtr(node).set(text);
		return true;
	}
	bool operator<<(nodePtr node, const wchar_t* value)
	{
		return node << std::wstring(value);
	}
	template <class TCon>
	bool operator>>(nodePtr node, TCon*& object)
	{
		if (!entityPtr::match(node))
			return false;
		TCon* ptr = entityPtr(node).getEntity<TCon>();
		if (ptr == nullptr)
		{
			object = nullptr;
			return false;
		}
		object = ptr;
		return true;
	}
	/*template <class TCon>
	bool operator>>(nodePtr node, TCon*& object)
	{
		if (!entityPtr::match(node))
			return false;
		TCon* ptr = entityPtr(node).getEntity<TCon>();
		if (ptr == nullptr)
		{
			object = nullptr;
			return false;
		}
		object = ptr;
		return true;
	}*/
}