#pragma once
#include "node.h"

#include <exception>
#include <string>
#include <vector>
#include <chrono>

namespace nechto
{
	struct exception : public std::exception
	{
		const nodePtr Where;

		exception(const std::string pWhat = "unknown error", nodePtr pWhere = nullNodePtr)
			:std::exception(pWhat.c_str()), Where(pWhere) {}

		exception(const char* const pWhat, nodePtr pWhere = nullNodePtr)
			:std::exception(pWhat, 0), Where(pWhere) {}
		virtual ~exception() {}

		virtual const char* what() const;
	};
}