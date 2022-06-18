#pragma once
#include "node.h"

namespace nechto
{
	bool hasSubType(nodePtr v1)
	{
		switch (v1->type.load())
		{
		case node::Hub:
			return false;
		case node::Variable:
			return true;
		case node::TypeCastOperator:
			return false;
		case node::MathOperator:
			return true;
		case node::ConditionalBranching:
			return false;
		case node::ExteralFunction:
			return false;
		case node::Tag:
			return true;
		default:
			assert(false);
		}
		return false;
	}
	bool hasStaticData(nodePtr v1)
	{
		switch (v1->type.load())
		{
		case node::Hub:
			return false;
		case node::Variable:
			return true;
		case node::TypeCastOperator:
			return false;
		case node::MathOperator:
			return false;
		case node::ConditionalBranching:
			return false;
		case node::ExteralFunction:
			return false;
		case node::Tag:
			return true;
		default:
			assert(false);
		}
		return false;
	}
	bool hasStaticAdData(nodePtr v1)
	{
		switch (v1->type.load())
		{
		case node::Hub:
			return false;
		case node::Variable:
			return false;
		case node::TypeCastOperator:
			return false;
		case node::MathOperator:
			return false;
		case node::ConditionalBranching:
			return false;
		case node::ExteralFunction:
			return true;
		case node::Tag:
			return true;
		default:
			assert(false);
		}
		return false;
	}
}
/*
	switch (v1->type.load())
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
		case node::ExteralFunction:
			return;
		case node::Tag:
			return;
		default:
			assert(false);
			}
*/