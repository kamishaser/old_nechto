#pragma once
#include "visualNodeFill.h"

namespace nechto::ide
{
	class vnDataUpdateH
	{
	public:
		nodePtr nBoardNode;
		vnDataUpdateH(nodePtr nbn)
			:nBoardNode(nbn) {}
		void update()
		{
			nodeBoard* nBoard = nodeBoard::getByNode(nBoardNode);
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