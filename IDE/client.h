#pragma once

#include "ideDisplay.h"
#include "nodeBoard.h"
#include "spreadHandler.h"
#include "autoExpandHandler.h"
#include "userH.h"
#include "vnDataUpdateH.h"
#include "checkAndStep.h"
#include "placementHandler.h"
#include "consistentGroup.h"


namespace nechto::ide
{
	//при удалении ноды надо удалить все итераторы
	//при группы надо удалить все итераторы
	//хабы должны удаляться не в deleteNode
	//оптимизация eraseHub в итераторе или нормальное удаление всех итераторов
	class client
	{
	public:
		nodeBoard* nBoard;
		ideDisplay* display;
		placementHandler plh;
		vnDataUpdateH vduh;
		autoExpandHandler aeh;
		consistentGroup mainGroup;
		userH uh;

		client(/*externalFunction::shEFS exFunSet*/)
			:nBoard(new nodeBoard),
			display(new ideDisplay(nBoard)),
			mainGroup(newExObjNode(), nBoard, glm::vec2{500, 500}, consistentGroup::arrangeMode()),
			plh(nBoard),
			vduh(nBoard),
			uh(nBoard, display),
			aeh(nBoard)			
		{
			IterIterConnect(group::firstEmptyPort(nBoard->taggedGroup()),
				connectionIterator(uh.cursored.get(), 1));

			nBoard->addNode(newNode(node::ExternalObject, 1), example());
			NumHubConnect(mainGroup.get(), nBoard->vNodeGroup(), 0);
		}
		int i = 0;
		bool update()
		{
			aeh.update();
			vduh.update();
			uh.update();
			std::this_thread::sleep_for(2000ms);
			mainGroup.mode.horisontal = false;// i & 1;
			mainGroup.mode.rightAlignment = true;// i & 1 << 1;
			mainGroup.mode.reverseDirection = false;// i & 1 << 2;
			mainGroup.mode.table4 = true;// i & 1 << 3;
			mainGroup.arrange(10);
			++i;
			return display->update();

		}

		nodePtr example()
		{
			nodePtr descriptionText = newNode(node::Text);
			text::set(descriptionText, std::wstring(L"Простейший алгоритм\n") +
				L"для разработки и тестирования" +
				L"системы базового отображения nechto");
			
			nodePtr vFirst= nullNodePtr;
			nodePtr vLast = nullNodePtr;
			for (int i = 0; i < 16; ++i)
			{
				nodePtr v1 = newNode(node::Variable, true);
				v1->setData<i64>(i);
				if (vLast.exist())
					HubHubConnect(v1, vLast);
				else
					vFirst = v1;
				vLast = v1;
			}
			return vFirst;
		}
	};

}

