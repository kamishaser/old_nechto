#pragma once
#include "graph.h"
#include "visualNodeFill.h"

namespace nechto::ide::handler
{
	class vnDataUpdateH : public graph::handler
	{
	public:
		vnDataUpdateH()
			:graph::handler(21ms, 25ms){}

		virtual ~vnDataUpdateH() {}

		virtual void update()
		{
			for (auto i1 = nGraph->nodes.begin(); i1 != nGraph->nodes.end(); ++i1)
			{
				vnText::fill(i1->second);
				vnShape::fill(i1->second);
				vnLightColor::fill(i1->second);
			}
		}
	};
}