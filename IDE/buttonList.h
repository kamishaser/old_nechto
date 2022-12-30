#pragma once
#include "button.h"
#include "namedExConGroup.h"
#include "visualGroup.h"
#include "GUI.h"

namespace nechto::ide
{
	//список кнопок
	class buttonList : public namedExConGroup
	{
		//є0 - группа кнопок
		//є1 - группа visualNode
		//є2 - последн€€ нажата€ нода
		//є3 - где используетс€
	public:
		//режим выбора. —обыти€ кнопки перехватывает mouseHandler и ставит lClicked
		bool choiseMode;
		using clickEventT = std::function<void(buttonList*)>;
		clickEventT clickEvent = nullptr;
		
		buttonList(nodePtr emptyExternalObject, visualGroup* vGroup,
			const std::wstring& name,
			bool choise = false)
			:namedExConGroup(emptyExternalObject, name), 
			choiseMode(choise)
		{
			NumNumConnect(node(), vGroup->node(), 1, 1);
		}
		buttonList(nodePtr emptyExternalObject, visualGroup* vGroup,
			const std::wstring& name, bool choise,
			std::initializer_list<std::wstring> ilist, 
			std::wstring firstActive = std::wstring())
			:buttonList(emptyExternalObject, vGroup, name, choise)			
		{
			assert(ilist.size() > 0);
			bool active = false;
			for (auto& bName : ilist)
			{
				auto button = new sharedButton(bName);
				addButton(button);
				if (!active && bName == firstActive)
				{
					click(button);
					active = true;
				}
			}
		}
		buttonList(nodePtr emptyExternalObject, visualGroup* vGroup,
			const std::wstring& name, bool choise,
			const std::vector<std::wstring>& ilist,
			std::wstring firstActive = std::wstring())
			:buttonList(emptyExternalObject, vGroup, name, choise)
		{
			assert(ilist.size() > 0);
			bool active = false;
			for (auto& bName : ilist)
			{
				auto button = new sharedButton(bName);
				addButton(button);
				if (!active && bName == firstActive)
				{
					click(button);
					active = true;
				}
			}
		}
		bool visible() const
		{
			if (!vNodeGroup())
				return false;
			return vNodeGroup()->node().connection(3).exist();
		}
		void hide()
		{
			assert(vNodeGroup());
			groupIterator gi(getGroup());
			do
			{
				auto button = getObject<sharedButton>(gi.get());
				if (button)
					hideButton(button);
			} while (gi.stepForward());
			nearestDisconnect(vNodeGroup()->node(), 3);
		}
		visualGroup* vNodeGroup() const
		{
			return getObject<visualGroup>(node().connection(1));
		}
		void show(nodeBoard* nBoard)
		{
			assert(vNodeGroup());
			nBoard->addGroup(getObjectPtr<visualGroup>(vNodeGroup()));

			groupIterator buttonGi(getGroup());
			do
			{
				auto button = getObject<sharedButton>(buttonGi.get());
				if (button)
					showButton(button, vNodeGroup(), nBoard);
			} while (buttonGi.stepForward());
		}
		void addButton(sharedButton* button)
		{
			IterIterConnect(firstEmptyGroupPort(getGroup()),
				portIterator(button->node(), 3));
			if (visible())
				showButton(button, vNodeGroup(), 
					getObject<nodeBoard>(vNodeGroup()->getNodeBoard()));
		}
		void resetButton(sharedButton* button)
		{
			if (visible())
				hideButton(button);
			nearestDisconnect(button->node(), 3);
		}
		bool containsButton(sharedButton* button)
		{
			return (button->node().connection(0) == getGroup());
		}
		sharedButton* lClicked()
		{
			return getObject<sharedButton>(node().connection(2));
		}
		bool isLClicked(sharedButton* button)
		{
			if (!(button && lClicked()))
				return false;
			return button->node() == lClicked()->node();
		}
		void resetLastClicked()
		{
			nearestDisconnect(node(), 2);
		}
		void click(sharedButton* button)
		{
			NumHubConnect(node(), button->node(), 2);
			if(clickEvent != nullptr)
				clickEvent(this);
		}
	private:
		void showButton(sharedButton* button, visualGroup* vGroup, nodeBoard* nBoard)
		{
			auto vNode = new visualNode(creator::createObject(1));
			//подключение ноды к кнопке
			NumNumConnect(button->node(), vNode->node(), 0, 0);
			//вставка ноды в visualGroup
			IterIterConnect(firstEmptyGroupPort(vGroup->getGroup()),
				portIterator(vNode->node(), 1));
			//подключение кнопки к interfaceBoard
			nBoard->addNode(getObjectPtr(vNode));

			vNode->nodeText = button->name;
		}
		void hideButton(sharedButton* button)
		{
			if (button->node().connection(0).exist())
				creator::deleteNode(button->node().connection(0));
		}
	public:
		const static std::wstring typeName;
		const static staticNodeOperationSet methodSet;
		virtual const std::wstring& getTypeName() const override
		{
			return typeName;
		}
		virtual ~buttonList()
		{
			hide();
		}
	};
	const std::wstring buttonList::typeName = L"nechtoIde.buttonList";
}