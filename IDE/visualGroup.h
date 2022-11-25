#pragma once
#include "namedExConGroup.h"
#include "visualNode.h"
#include "rect.h"
namespace nechto::ide
{

	//соединение 0 - подконтрольная группа нод (обр номер 0)
	//connection 1 - использующий группу
	//cоединение 3 - vGroupGroup от nodeBoard
	//набор правил и действий (что делать, если допустим на ноду нажал пользователь)
	//группа visualNode
	struct visualGroup :public namedExConGroup
	{
		rect frame;
		visualGroup(nodePtr emptyExternalObject, const std::u16string& name,
			glm::vec2 startPoint = glm::vec2{ 0,0 })
			:namedExConGroup(emptyExternalObject, name),
			frame(startPoint, glm::vec2{ 1.f, 1.f }) {}
		nodePtr vNodeGroup() const
		{
			return getConnection(0);
		}
		nodePtr getNodeBoard() const
		{
			nodePtr temp = getConnection(3);
			if (!temp.exist())
				return nullNodePtr;
			return temp->connection[0];
		}
		bool contains(visualNode* vNode) const
		{
			return (vNodeGroup() == vNode->getConnection(0));
		}
		//обновить группу
		virtual void update() {}
		//добавляет ноду в первый свободный порт
		void addNode(visualNode* vNode) const
		{
			//vNodeGroup может содержать ноды только из одного nodeBoard
			assert(vNode->getNodeBoard() == getNodeBoard());
			IterIterConnect(connectionIterator(vNode->get(), 1),
				group::firstEmptyPort(vNodeGroup()));
		}
		void addGroup(visualGroup* vGroup) const
		{
			//vNodeGroup может содержать ноды только из одного nodeBoard
			assert(vGroup->getNodeBoard() == getNodeBoard());
			IterHubConnect(group::firstEmptyPort(vNodeGroup()), vGroup->get());
		}
		i64 numberOfVNodes() const
		{
			return group::numberOfMembers(vNodeGroup());
		}
		bool allNodeOnOneNodeBoard() const
		{
			nodePtr nBoardNode = getNodeBoard();
			groupIterator gi(vNodeGroup());
			do
			{
				auto vNode = visualNode::getByNode(gi.get());
				if (vNode)
				{
					if (vNode->getNodeBoard() != nBoardNode)
						return false;
				}
			} while (gi.stepForward());
			return true;
		}

		//получение рамки ноды или группы
		static rect* getRect(nodePtr v1)
		{
			if (!v1.exist())
				return nullptr;
			if (!typeCompare(v1, node::ExternalObject))
				return nullptr;
			auto vNode = visualNode::getByNode(v1);
			if (vNode)
				return &vNode->frame;
			auto vGroup = visualGroup::getByNode(v1);
			if (vGroup)
				return &vGroup->frame;
			return nullptr;
		}

		//////////////////////////////////////////////////////////////
		virtual ~visualGroup()
		{
			if (vNodeGroup().exist())
				deleteNode(vNodeGroup());
		}
		/*получение указателя на visualGroup по объекту.
		Возвращает nullptr при несоответствии*/
		static visualGroup* getByNode(nodePtr v1)
		{
			if (!v1.exist())
				return nullptr;
			if (v1->getType() != node::ExternalObject)
				return nullptr;
			return dynamic_cast<visualGroup*>(v1->getData<externalObject*>());
		}
		const static std::u16string typeName;
		const static staticNodeOperationSet methodSet;
		const static connectionRule cRule;
		virtual const std::u16string& getTypeName() const override
		{
			return typeName;
		}
		virtual const operation& getMethod(char number)const override
		{
			return methodSet.getOperation(number);
		}
		virtual const conRule& getConnectionRule()const override
		{
			return cRule;
		}
	};
	const std::u16string visualGroup::typeName = u"nechtoIde.visualGroup";
	const connectionRule visualGroup::cRule = connectionRule{};
	const staticNodeOperationSet visualGroup::methodSet
	{
		/*namedOperation(u"nothing", operation{
				connectionRule(conRule::ExternalObject, conRule::Input, nullptr),
				[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				return true;
			}})*/
	};
	
}