#pragma once
#include "node.h"
#include "hub.h"

namespace nechto
{
	//�� ����������
	struct hubIterator //����� �������� ������� �����. �������� ��� ���������, ��� � � �������� ���������
	{
		nodePtr currentHub;
		nodePtr mainNode;
		char position = 0;//��������: ������� ������ ����� pos()

		hubIterator() {}
		hubIterator(nodePtr curHub, nodePtr base, char pos)
			:currentHub(curHub), mainNode(base), position(pos) {}
		
		//�������, �� ������� ��������� ��������
		/*const std::atomic<nodePtr>* operator->() const
		{
			return currentHub->connection[pos()];
		}
		const std::atomic<nodePtr>* operator*() const
		{
			return currentHub->connection[pos()];
		}*/
		std::atomic<nodePtr>& operator->()
		{
			return currentHub->connection[pos()];
		}
		std::atomic<nodePtr>& operator*()
		{
			return currentHub->connection[pos()];
		}
		nodePtr get()const 
		{
			return currentHub->connection[pos()].load();
		}
		//������� ��������� � ����. ����� ���������� �� 0 �� 3
		char pos() const
		{
			return position & static_cast<char>(3);
		}
		//�������� ������ �� ���� ���������
		void pull(nodePtr v1)
		{
			assert(typeCompare(v1, node::Pointer));
			assert(!subtypeCompare(v1, pointer::Reference));
			currentHub = v1->connection[0];
			mainNode = v1->connection[1];
			position = v1->getData<char>();
		}
		//��������� ������ � ���� ��������
		void push(nodePtr v1) const
		{
			assert(typeCompare(v1, node::Pointer));
			assert(!subtypeCompare(v1, pointer::Reference));
			v1->connection[0] = currentHub;
			v1->connection[1] = mainNode;
			v1->setData<char>(position);
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
		connectionIterator(nodePtr v1)
			:hubIterator(v1, v1, 0) {}
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
			nodePtr previousNode =
				currentHub->getData<std::pair<nodePtr, nodePtr>>().first;
			//����  ���� �� ���������� - ������� � �������� ����
			if (previousNode.exist())
			{
				currentHub = previousNode;
				return true;
			}
			else
			{
				nodePtr hubIterator = currentHub;
				i64 pos = 3;
				while (true)
				{
					nodePtr next = hubIterator->hubConnection;
					if (!next.exist())
						break;
					++pos;
					hubIterator = next;
				}
				currentHub = hubIterator;
				position = pos;
				return false;
			}
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
			hub::erase(currentHub, mainNode);
			
		}
	};
	class groupIterator : public hubIterator
	{
	public:
		groupIterator(nodePtr v1)
			:hubIterator(v1->connection[0], v1, 0) 
		{
			assert(typeCompare(v1, node::Group));
		}

		bool GoToNextHub()
		{
			currentHub = currentHub->hubConnection.load();
			return (currentHub != mainNode->connection[0]);
		}
		bool GoToPreviousHub()
		{
			bool result = currentHub != mainNode->connection[0];
			currentHub = currentHub->getData<std::pair<nodePtr, nodePtr>>().first;
			return result;
		}
		bool stepForward()
		{
			++position;
			if (!(position & 3ll))//���� ������� == 0, ������� � ��������� ���
			{
				return GoToNextHub();
			}
			else
				return true;
		}
		bool stepBack()
		{
			bool result;
			if (!(position & 3ll))//���� ������� ����� 0, ������� � ���������� ���
				result = GoToPreviousHub();
			else
				result = true;
			--position;
			return result;
		}
		nodePtr firstHub()
		{
			return mainNode->connection[0];
		}

		bool atFirst()
		{
			if (!(position & 3ll))
				return false;
			nodePtr begin = firstHub();
			if (currentHub != begin->connection[0])
				return false;
			return true;
		}
		void insertHub()
		{
			mainNode->data.fetch_add(1);
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
			if (mainNode->connection[0] = currentHub)
				mainNode->connection[0] = hub::next(currentHub);
			hub::erase(currentHub, mainNode);
		}
	};
}