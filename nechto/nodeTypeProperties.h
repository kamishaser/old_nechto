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
		case node::ExternalFunction:
		case node::ExternalConnection:
		case node::Group:
			return false;
		case node::Variable:
		case node::MathOperator:
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
		case node::ExternalFunction:
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
		case node::ExternalFunction:
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
		case node::ExternalFunction:
			return;
		case node::Text:
			return;
		default:
			assert(false);
			}
*/