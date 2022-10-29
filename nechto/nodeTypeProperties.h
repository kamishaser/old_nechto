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
		case node::Group:
			return false;
		default:
			return true;
		}
	}
	bool hasData(nodePtr v1)
	{
		switch (v1->getType())
		{
		case node::Variable:
		case node::Text:
		case node::Group:
		case node::Pointer:
			return true;
		default:
			return false;
		}
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