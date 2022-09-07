#pragma once
#include "ideDisplay.h"
#include "textOut.h"

namespace nechto::ide
{
	bool vNodeContainsPoint(glm::vec2 position, glm::vec2 size, glm::vec2 point)
	{
		glm::vec2 p = position;
		glm::vec2 s = size / 2.0f;

		float top = p.y - s.y;
		float bottom = p.y + s.y;
		float left = p.x - s.x;
		float right = p.x + s.x;
		assert(bottom > top);
		assert(right > left);
		return ((point.x > left && point.x < right) &&
			(point.y > top && point.y < bottom));
	}
	class userH
	{
	public:
		nodePtr ideDisplayNode;
		nodePtr nBoardNode;

		namedExCon cursored;
		glm::vec2 lastCPos;


		//externalObject moveCursored =
		//	externalObject(createVariable(0ll), L"moveCursored");
		//glm::vec2 offset;
		bool last = false;
	
		userH(nodePtr nbn, nodePtr idn)
		:nBoardNode(nbn), ideDisplayNode(idn),
			cursored(L"#nechtoIde.cursored"),
			lastCPos(ideDisplay::getByNode(ideDisplayNode)->getCursorPosition()){}

		void update()
		{
			updateCursored();
		}
	private:
		void updateCursored()
		{
			nodePtr last = cursored.getConnection(0);
			if (last.exist())
				numDisconnect(cursored.get(), 0);
			nodeBoard* nBoard = nodeBoard::getByNode(nBoardNode);
			ideDisplay* display = ideDisplay::getByNode(ideDisplayNode);

			groupIterator i1(nBoard->vNodeGroup());
			do
			{
				auto* vNode = visualNode::getByNode(i1.get());
				if (!vNode)
					continue;
				if (vNodeContainsPoint(vNode->position,
					vNode->size, display->getCursorPosition()))
				{
					if (vNode->get().exist())
					{
						if (vNode->get() != last)
						{
							std::wcout << L"cursored: " << nodeProperties(vNode->getConnection(0)) << std::endl;
						}
						NumHubConnect(cursored.get(), vNode->get(), 0);
					}
				}
			} while (i1.stepForward());
		}
	};
}