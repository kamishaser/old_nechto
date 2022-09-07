#pragma once
#include "node.h"
#include "hub.h"

namespace nechto
{
	struct hubIterator //����� �������� ������� �����. �������� ��� ���������, ��� � � �������� ���������
	{
	protected:
		char position = 0;//��������: ������� ������ ����� pos()
	public:
		nodePtr currentHub;
		nodePtr mainNode;
		hubIterator() {}
		hubIterator(nodePtr curHub, nodePtr base, char pos)
			:currentHub(curHub), mainNode(base), position(pos) {}
		hubIterator(nodePtr mainNode, pointer::hubPosPair hpp)
			:currentHub(hpp.first), mainNode(mainNode), position(hpp.second) {}
		
		//�������, �� ������� ��������� ��������
		/*const std::atomic<nodePtr>* operator->() const
		{
			return currentHub->connection[pos()];
		}
		const std::atomic<nodePtr>* operator*() const
		{
			return currentHub->connection[pos()];
		}*/
		auto& operator*()
		{
			return currentHub->connection[pos()];
		}
		nodePtr get()const 
		{
			return currentHub->connection[pos()];
		}
		void set(nodePtr v1)
		{
			IterHubConnect(*this, v1);
		}

		//������� ��������� � ����. ����� ���������� �� 0 �� 3
		char pos() const
		{
			return position & static_cast<char>(3);
		}
		//�������� ������ �� ���� ���������
		bool pull(nodePtr v1)
		{
			assert(typeCompare(v1, node::Pointer));
			assert(!subtypeCompare(v1, pointer::Simple));
			auto hpp = v1->getData<pointer::hubPosPair>();
			currentHub = hpp.first;
			if (!currentHub.exist())
				return false;
			mainNode = hub::getMain(currentHub);
			position = hpp.second;
			
			return true;
		}
		//��������� ������ � ���� ��������
		void push(nodePtr v1) const
		{
			assert(typeCompare(v1, node::Pointer));
			assert(!subtypeCompare(v1, pointer::Simple));
			if (v1->connection[0] != mainNode)
				NumHubConnect(v1, mainNode, 0);
			v1->setData(pointer::hubPosPair(currentHub, position));
		}
		//������������ ������������ � v1. ���������� ������ ����������
		nodePtr oneSideConnect(nodePtr v1)
		{
			return currentHub->connection[pos()].exchange(v1);
		}
		//������������ �����������. ���������� ������ ����������
		nodePtr oneSideDisconnect()
		{
			return oneSideConnect(nullNodePtr);
		}
		//����������� �����������. ���������� ������ ����������
		void disconnect(); // ����������� � nodeOperations.h
		bool operator==(const hubIterator& i2) const
		{
			return (mainNode == i2.mainNode && pos() == i2.pos() &&
				currentHub == i2.currentHub);
		}
		bool operator!=(const hubIterator& i2) const
		{
			return (mainNode != i2.mainNode || pos() != i2.pos() ||
				currentHub != i2.currentHub);
		}
		//����� ������������� ���������� � �������� ����� �������
		static bool exchangeConnections(hubIterator& fI, hubIterator& sI)
		{
			if (fI.mainNode != sI.mainNode)
				return false;
			nodePtr temp = fI.get();
			fI.currentHub->connection[fI.pos()] = sI.currentHub->connection[sI.pos()].load();
			sI.currentHub->connection[sI.pos()] = temp;
			return true;
		}
	};

	class connectionIterator : public hubIterator
	{
	public:
		connectionIterator(nodePtr v1, char pos = 0)
			:hubIterator(v1, v1, pos) 
		{
			assert(pos < 4);
		}
		connectionIterator(nodePtr currentHub, nodePtr mainNode, char pos)
			:hubIterator(currentHub, mainNode, pos){}
		connectionIterator(nodePtr mainNode, pointer::hubPosPair hpp)
			:hubIterator(hpp.first, mainNode, hpp.second) {}
		bool GoToNextHub()
		{
			nodePtr nextNode = currentHub->hubConnection.load();
			//���� ���������� ���� �� ���������� - ������� � �������� ����
			if (nextNode.exist())
			{
				currentHub = nextNode;
				return true;
			}
			else
			{
				currentHub = mainNode;
				position = 0;
				return false;
			}
		}
		bool GoToPreviousHub()
		{
			if (typeCompare(currentHub, node::Hub))
			{
				currentHub = hub::previous(currentHub);
			}
			else
			{
				nodePtr hubIter = currentHub;
				char pos = 3;
				while (true)
				{
					nodePtr next = hubIter->hubConnection;
					if (!next.exist())
						break;
					++pos;
					hubIter = next;
				}
				currentHub = hubIter;
				position = pos;
				return false;
			}
			return currentHub != mainNode->connection[0];
		}

		bool stepForward()
		{
			++position;
			if (!(position & 3ll))
				return true;
			else
				return GoToNextHub();
		}
		bool stepBack()
		{
			--position;
			if ((position & 3ll) != 3)
				return true;//���� ����������� ���������� � �������� ������ ���� - �� ����
			else
				return GoToPreviousHub();
		}
		//�������� �� ������ ��������
		bool atFirst()
		{
			if (!pos())
				return false;
			if (currentHub != mainNode)
				return false;
			return true;
		}
		//�������� �� ��������� ��������
		bool atLast()
		{
			if (pos() != 3)
				return false;
			if (currentHub->hasHub())
				return false;
			return true;
		}
		//�������� ��� ����� ��������
		void insertHub()
		{
			hub::insert(currentHub, mainNode);
		}
		//������� ������� ���. ��������� ��� ����������� �� ���� ��������� �� ���������
		void eraseHub()
		{
			assert(currentHub != mainNode);
			//���������� ��� ������������ ���� � ������� ���������, ������������ �� ��������� ���
			connectionIterator conIter(mainNode);
			do
			{
				for (int i = 0; i < 4; ++i)
				{
					nodePtr temp = conIter.get();
					if (temp.exist())
						if (typeSubtypeCompare(temp, node::Pointer, pointer::ConIter))
						{//��������� �� ���������
							nodePtr hub = temp->connection[0];
							if (hub == currentHub)
								temp->connection[0] = hub::next(hub);
						}
				}
			} while (conIter.stepForward());
			nodePtr next = hub::next(currentHub);
			hub::erase(currentHub, mainNode);
			currentHub = next;
		}
	};
	class groupIterator : public hubIterator
	{
	public:
		groupIterator(nodePtr v1, char pos = 0)
			:hubIterator(v1->getData<nodePtr>(), v1, pos)
		{
			assert(pos < 4);
			assert(typeCompare(v1, node::Group));
		}
		groupIterator(nodePtr currentHub, nodePtr mainNode, char pos)
			:hubIterator(currentHub, mainNode, pos) 
		{
			assert(pos < 4);
			assert(typeCompare(mainNode, node::Group));
		}
		groupIterator(nodePtr mainNode, pointer::hubPosPair hpp)
			:hubIterator(hpp.first, mainNode, hpp.second) {}

		bool atFirstHub()
		{
			return currentHub == mainNode->getData<nodePtr>();
		}
		bool GoToNextHub()
		{
			currentHub = hub::next(currentHub);
			return atFirstHub();
		}
		bool GoToPreviousHub()
		{
			currentHub = hub::previous(currentHub);
			return atFirstHub();
		}
		bool stepForward()
		{
			++position;
			if (!(position & 3ll))//���� ������� == 0, ������� � ��������� ���
			{
				return !GoToNextHub();
			}
			else
				return true;
		}
		bool stepBack()
		{
			bool result;
			if (position & 3ll)//���� ������� ����� 0, ������� � ���������� ���
				result = true;
			else
				result = !GoToPreviousHub();
			--position;
			return result;
		}
		nodePtr firstHub()
		{
			return mainNode->getData<nodePtr>();
		}

		bool atFirst()
		{
			if (!(position & 3ll))
				return false;
			nodePtr begin = firstHub();
			return atFirstHub();
		}
		void insertHub()
		{
			hub::insert(currentHub, mainNode);
		}
		void eraseHub()
		{
			assert(currentHub != mainNode);
			if (mainNode->data.fetch_add(-1) == 1)
			{//������ ������� ��������� ���
				mainNode->data.fetch_add(1);
				return;
			}
			//���������� ��� ������������ ���� � ������� ���������, ������������ �� ��������� ���
			connectionIterator conIter(mainNode);
			do
			{
				for (int i = 0; i < 4; ++i)
				{
					nodePtr temp = conIter.get();
					if (temp.exist())
						if (typeSubtypeCompare(temp, node::Pointer, pointer::GroupIter))
						{//��������� �� ���������
							nodePtr hub = temp->connection[0];
							if (hub == currentHub)
								temp->connection[0] = hub::next(hub);
						}
				}
			} while (conIter.stepForward());
			if (atFirstHub())
				mainNode->setData<nodePtr>(hub::next(currentHub));
			nodePtr next = hub::next(currentHub);
			hub::erase(currentHub, mainNode);
			currentHub = next;
		}
	};
	bool pointer::set(nodePtr pointer, nodePtr v1)
	{
		if (pointer->getSubtype() == pointer::Simple)
		{
			NumHubConnect(pointer, v1, 0);
		}
		else
		{
			hubIterator i0(pointer->connection[0],
				pointer->getData<pointer::hubPosPair>());
			i0.set(v1);
		}
		return true;
	}
}