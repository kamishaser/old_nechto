#pragma once
#include "namedExCon.h"

#include "visualNode.h"
#include "visualConnection.h"
namespace nechto::ide
{
	glm::vec2 randomPos(glm::vec2 border)
	{
		return glm::vec2
		(
			static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / border.x)),
			static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / border.y))
		);
	}
	glm::vec2 randomOffset(float max)
	{
		return glm::vec2
		(
			static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / max / 2)) - max,
			static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / max / 2)) - max
		);
	}
	struct nodeBoard : public externalObject
	{
		nodeBoard()//!!!только в куче, только через new!!!
			:externalObject(newNode(node::ExternalObject, 1))
		{
			NumNumConnect(get(), newNode(node::Group), 0, 1);
			NumNumConnect(get(), newNode(node::Group), 1, 1);
			NumNumConnect(get(), newNode(node::Group), 2, 1);
		}
			nodePtr vNodeGroup()
		{
			return exObj->connection[0];
		}
		nodePtr vConnectionGroup()
		{
			return exObj->connection[1];
		}
		nodePtr taggedGroup()//група различным образом помеченных нод и соединений
		{
			return exObj->connection[2];
		}
		
		visualNode* addNode(nodePtr v1 = nullNodePtr)
		{
			visualNode* vn = new visualNode(vNodeGroup(), v1);
			vn->position += randomOffset(400);
			return vn;
		}
		visualConnection* addConnection(visualNode* vn1, visualNode* vn2)
		{
			visualConnection* vc = new visualConnection(vConnectionGroup(), vn1, vn2);
			return vc;
		}
		bool onThisBoard(visualNode* vNode)
		{
			assert(vNode);
			return vNodeGroup() == vNode->get()->connection[3];
		}
		bool onThisBoard(visualConnection* vCon)
		{
			assert(vCon);
			return vConnectionGroup() == vCon->get()->connection[3];
		}
		bool connected(visualNode* vNode1, visualNode* vNode2)
		{
			if (!onThisBoard(vNode1) || !onThisBoard(vNode2))
				return false;
			connectionIterator i1(vNode1->get());
			do
			{
				visualConnection* vscon = visualConnection::getByNode(i1.get());
				if (vscon)
				{
					if (i1.get()->connection[0] == vNode2->get() ||
						i1.get()->connection[1] == vNode2->get())
						return true;
				}
			} while (i1.stepForward());
			return false;
		}
		/*возвращает nullptr (интерпретируется в false) если
		на данной доске нет visualNode указывающей на эту ноду
		//иначе возвращает указатель на visualNode*/
		visualNode* visualized(nodePtr v1)
		{
			if (!v1.exist())
				return nullptr;
			connectionIterator i1(v1);
			do
			{
				visualNode* vNode = visualNode::getByNode(i1.get());
				if (vNode)
					if (onThisBoard(vNode))
						return vNode;
			} while (i1.stepForward());
			return nullptr;
		}
		static nodeBoard* getByNode(nodePtr v1)
		{
			if (!v1.exist())
				return nullptr;
			if (v1->getType() != node::ExternalObject)
				return nullptr;
			auto exObj = v1->getData<externalObject*>();
			if (exObj == nullptr)
				return nullptr;
			if (exObj->getTypeName() != typeName)
				return nullptr;
			return dynamic_cast<nodeBoard*>(exObj);
		}
		const static std::wstring typeName;
		virtual const std::wstring& getTypeName() const override
		{
			return typeName;
		}
	};
	const std::wstring nodeBoard::typeName = L"nechtoIde.nodeBoard";
}