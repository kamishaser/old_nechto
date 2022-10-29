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
			NumNumConnect(get(), vGroup->get(), 1, 1);
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
			return vNodeGroup()->getConnection(3).exist();
		}
		void hide()
		{
			assert(vNodeGroup());
			groupIterator gi(getGroup());
			do
			{
				auto button = sharedButton::getByNode(gi.get());
				if (button)
					hideButton(button);
			} while (gi.stepForward());
			numDisconnect(vNodeGroup()->get(), 3);
		}
		visualGroup* vNodeGroup() const
		{
			return visualGroup::getByNode(getConnection(1));
		}
		void show(nodeBoard* nBoard)
		{
			assert(vNodeGroup());
			nBoard->addGroup(vNodeGroup());

			groupIterator buttonGi(getGroup());
			do
			{
				auto button = sharedButton::getByNode(buttonGi.get());
				if (button)
					showButton(button, vNodeGroup(), nBoard);
			} while (buttonGi.stepForward());
		}
		void addButton(sharedButton* button)
		{
			IterIterConnect(group::firstEmptyPort(getGroup()),
				connectionIterator(button->get(), 3));
			if (visible())
				showButton(button, vNodeGroup(), 
					nodeBoard::getByNode(vNodeGroup()->getNodeBoard()));
		}
		void resetButton(sharedButton* button)
		{
			if (visible())
				hideButton(button);
			numDisconnect(button->get(), 3);
		}
		bool containsButton(sharedButton* button)
		{
			return (button->getConnection(0) == getGroup());
		}
		sharedButton* lClicked()
		{
			return sharedButton::getByNode(getConnection(2));
		}
		bool isLClicked(sharedButton* button)
		{
			if (!(button && lClicked()))
				return false;
			return button->get() == lClicked()->get();
		}
		void resetLastClicked()
		{
			numDisconnect(get(), 2);
		}
		void click(sharedButton* button)
		{
			NumHubConnect(get(), button->get(), 2);
			if(clickEvent != nullptr)
				clickEvent(this);
		}
	private:
		void showButton(sharedButton* button, visualGroup* vGroup, nodeBoard* nBoard)
		{
			auto vNode = new visualNode(newExObjNode());
			//подключение ноды к кнопке
			NumNumConnect(button->get(), vNode->get(), 0, 0);
			//вставка ноды в visualGroup
			IterIterConnect(group::firstEmptyPort(vGroup->getGroup()),
				connectionIterator(vNode->get(), 1));
			//подключение кнопки к interfaceBoard
			nBoard->addNode(vNode);

			vNode->nodeText = button->name;
		}
		void hideButton(sharedButton* button)
		{
			if (button->getConnection(0).exist())
				deleteNode(button->getConnection(0));
		}
	public:
		static buttonList* getByNode(nodePtr v1)
		{
			if (!v1.exist())
				return nullptr;
			if (v1->getType() != node::ExternalObject)
				return nullptr;
			return dynamic_cast<buttonList*>(v1->getData<externalObject*>());
		}
		const static std::wstring typeName;
		const static staticNodeOperationSet methodSet;
		const static connectionRule cRule;
		virtual const std::wstring& getTypeName() const override
		{
			return typeName;
		}
		virtual const conRule& getConnectionRule()const override
		{
			return cRule;
		}
		virtual ~buttonList()
		{
			hide();
		}
	};
	const std::wstring buttonList::typeName = L"nechtoIde.buttonList";
	const connectionRule buttonList::cRule = connectionRule{};
}