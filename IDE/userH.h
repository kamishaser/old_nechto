#pragma once
#include "ideDisplay.h"
#include "textOut.h"
#include "graph.h"

namespace nechto::ide::handler
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
	class userH : public graph::handler
	{
		std::shared_ptr<ideDisplay> display;

		externalConnection cursored;
		glm::vec2 lastCPos;


		//externalConnection moveCursored =
		//	externalConnection(createVariable(0ll), L"moveCursored");
		glm::vec2 offset;
		bool last = false;
	public:
		userH(std::shared_ptr<ideDisplay> d)
		:graph::handler(20ms, 25ms), display(d), lastCPos(d->getCursorPosition()),
		cursored(L"nechtoIDE.cursoredNode", newNode(node::ExternalConnection)) {}

		virtual ~userH() 
		{
			cursored.deleteExConNode();
		}

		virtual void update()
		{
			updateCursored();
		}
	private:
		void updateCursored()
		{
			nodePtr last = cursored.getConnection(0);
			if (last.exist())
				numDisconnect(cursored.get(), 0);
			for (auto i1 = nGraph->nodes.begin(); i1 != nGraph->nodes.end(); ++i1)
			{
				if (vNodeContainsPoint(i1->second.position,
					i1->second.size, display->getCursorPosition()))
				{
					if (i1->second.get().exist())
					{
						if (i1->second.get() != last)
						{
							std::wcout << L"cursored: " << nodeProperties(i1->first) << std::endl;
						}
						NumHubConnect(cursored.get(), i1->second.get(), 0);
					}
				}
			}
		}
	};
}