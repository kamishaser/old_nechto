#pragma once
#include "visualNodeFill.h"
#include "display.h"

namespace nechto::ide
{
	class vnDataUpdateH
	{
	public:
		display& dp;
		vnDataUpdateH(display& dplay)
			:dp(dplay) {}
		void update()
		{
			updateOneBoard(&dp.workBoard);
			updateOneBoard(&dp.interfaceBoard);
		}
	private:
		void updateOneBoard(nodeBoard* nBoard)
		{
			groupIterator i1(nBoard->vNodeGroup());
			do
			{
				visualNode* vNode = visualNode::getByNode(i1.get());
				if (vNode)
				{
					if (i1.get()->connection[0].load().exist())
					{
						vnText::fill(vNode);
						vnShape::fill(vNode);
					}
					vnLightColor::fill(vNode);
				}
			} while (i1.stepForward());
		}
	};
}