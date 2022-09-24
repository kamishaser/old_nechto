#pragma once
#include "visualNodeFill.h"

namespace nechto::ide
{
	class vnDataUpdateH
	{
	public:
		nodeBoard* nBoard;
		vnDataUpdateH(nodeBoard* nbn)
			:nBoard(nbn) {}
		void update()
		{
			assert(nBoard);
			groupIterator i1(nBoard->vNodeGroup());
			do
			{
				visualNode* vNode = visualNode::getByNode(i1.get());
				if (vNode && (i1.get()->connection[0].load().exist()))
				{
					vnText::fill(vNode);
					vnShape::fill(vNode);
					vnLightColor::fill(vNode);
				}
			} while (i1.stepForward());
		}
	};
}