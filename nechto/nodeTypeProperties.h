#pragma once
#include "node.h"

namespace nechto
{
	bool hasSubType(nodePtr v1)
	{
		switch (v1->getType())
		{
		case node::Hub:
		case node::ConditionalBranching:
		case node::Method:
		case node::Group:
			return false;
		case node::Variable://0 - double, 1 - int64
		case node::MathOperator:
		case node::ExternalObject://0 - не является единственным владельцем, 1 - unique_ptr
		case node::Text:
		case node::Pointer:
			return true;
		default:
			assert(false);
		}
		return false;
	}
	bool hasData(nodePtr v1)
	{
		switch (v1->getType())
		{
		case node::Hub:
			return false;
		case node::Variable:
			return true;
		case node::MathOperator:
			return false;
		case node::ConditionalBranching:
			return false;
		case node::Method:
			return false;
		case node::Text:
			return true;
		default:
			assert(false);
		}
		return false;
	}
	bool hasStaticAdData(nodePtr v1)
	{
		switch (v1->getType())
		{
		case node::Hub:
			return false;
		case node::Variable:
			return false;
		case node::MathOperator:
			return false;
		case node::ConditionalBranching:
			return false;
		case node::Method:
			return true;
		case node::Text:
			return true;
		default:
			assert(false);
		}
		return false;
	}
}
/*
	switch (v1->getType())
		{
		case node::Hub:
			return;
		case node::Variable:
			return;
		case node::TypeCastOperator:
			return;
		case node::MathOperator:
			return;
		case node::ConditionalBranching:
			return;
		case node::Method:
			return;
		case node::Text:
			return;
		default:
			assert(false);
			}
*/