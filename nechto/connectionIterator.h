#pragma once
#include "node.h"

namespace nechto
{
	bool typeSubtypeCompare(nodePtr v1, char type, char subtype);
	bool typeCompare(nodePtr v1, char type);

	class hubIterator //����� �������� ������� �����. �������� ��� ���������, ��� � � �������� ���������
	{
	public:
		struct iteratorData
		{
			char position;
		};
		static_assert(sizeof(iteratorData) <= 8);
		nodePtr currentHub = nullNodePtr;
		nodePtr mainNode = nullNodePtr;
		iteratorData iterdat;

		hubIterator(nodePtr curHub, nodePtr base, iteratorData idat)
			:currentHub(curHub), mainNode(base), iterdat(idat) {}
		hubIterator(nodePtr v1)
		{
			pull(v1);
		}
		bool stepForward()
		{
			++iterdat.position;
			if ((iterdat.position & 3ll) != 0)
				return true;//���� ����������� ���������� � �������� ������ ���� - �� ����
			else
			{
				nodePtr nextNode = currentHub->hubConnection.load();
				//���� ���������� ���� �� ����������� - ������� � �������� ����
				if (nextNode.exist())
				{
					currentHub = nextNode;
					return true;
				}
				else//� �������� ���� ����� �������� � ������ ���� ������� �� �����������
				{
					currentHub = mainNode;
					position = 0;
					return false;
				}
			}
		}
		bool stepBack()
		{
			--position;
			if ((position & 3ll) != 0)
				return true;//���� ����������� ���������� � �������� ������ ���� - �� ����
			else
			{
				nodePtr previousNode =
					currentHub->getData<std::pair<nodePtr, nodePtr>>().first;
				//����  ���� �� ���������� - ������� � �������� ����
				if (previousNode.exist())
				{
					currentHub = previousNode;
					return true;
				}
				else//� �������� ���� ����� �������� � ������ ���� ������� �� �����������
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
		}
		nodePtr get() const
		{
			return currentHub->connection[position & 3ll];
		}
		//�������� ������ �� ���� ���������
		void pull(nodePtr v1)
		{
			assert(typeCompare(v1, node::Pointer));
			assert(!subtypeCompare(v1, pointer::Reference));
			currentHub = v1->connection[0];
			mainNode = v1->connection[1];
			iterdat = v1->getData<iteratorData>();
		}
		//��������� ������ � ���� ��������
		void push(nodePtr v1)
		{
			assert(typeCompare(v1, node::Pointer));
			assert(!subtypeCompare(v1, pointer::Reference));
			v1->connection[0] = currentHub;
			v1->connection[1] = mainNode;
			v1->setData<iteratorData>(iterdat);
		}
		nodePtr oneSideDisconnect()
		{
			return currentHub->connection[position & 3ll].exchange(nullNodePtr);
		}

	};

	class connectionIterator : public hubIterator
	{
		void notifyAllIterators(nodePtr mainNode, std::function<void(nodePtr)> notify)
		{
			nodePtr hubIter = mainNode;//
			while (hubIter.exist())
			{
				for (int i = 0; i < 4; ++i)
				{
					nodePtr temp = hubIter->connection[i];
					if (temp.exist())
						if (typeSubtypeCompare(temp, node::Pointer, pointer::ConIter))
							if (temp->connection[1] == mainNode)
								notify(temp);
				}
				hubIter = hubIter->hubConnection;
			}
		}
	public:
		//�������� �� ������ ��������
		bool atFirst()
		{
			if (!(position & 3ll))
				return false;
			if (currentHub != mainNode)
				return false;
			return true;
		}
		//�������� �� ��������� ��������
		bool atLast()
		{
			if ((position & 3ll) != 3)
				return false;
			if (currentHub->hasHub())
				return false;
			return true;
		}
		//�������� ��� ����� ��������
		bool insertHub()
		{
			hub::insert(currentHub, mainNode);
		}
		//������� ������� ���
		bool eraseHub()
		{
			hub::erase(currentHub, mainNode);
		}
	}
	class arrayIterator
	{
		void notifyAllIterators(nodePtr mainNode, std::function<void(nodePtr)> notify)
		{
			nodePtr hubIter = mainNode;//
			while (hubIter.exist())
			{
				for (int i = 0; i < 4; ++i)
				{
					nodePtr temp = hubIter->connection[i];
					if (temp.exist())
						if (typeSubtypeCompare(temp, node::Pointer, pointer::ArrayIter))
							if (temp->connection[1] == mainNode)
								notify(temp);
				}
				hubIter = hubIter->hubConnection;
			}
		}
	public:
		nodePtr mainNode = nullNodePtr;
		nodePtr currentHub = nullNodePtr;
		i64 position;

		nodePtr firstHub()
		{
			return mainNode->connection[0];
		}
		nodePtr lastHub()
		{
			return mainNode->connection[1]
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
		bool atLast()
		{
			if ((position & 3ll) != 3)
				return false;
			nodePtr end = lastHub();
			if (currentHub != end->connection[0])
				return false;
			return true;
		}
		bool insertHub()
		{
			nodePtr end = mainNode->connection[1];
			//������� ��� ��������� � �������
			bool lastHub = currentHub == end->connection[0];
			nodePtr newHub = hub::insert(currentHub, mainNode);
			
			if (lastHub)//����������� ����� �������
			{
				end->connection[0] = newHub;
				end->setData(end->getData<i64>() + 4);
			}
			else//�������� ����
			{
				notifyAllIterators(mainNode,
					[&](nodePtr iter)
					{
						if()
					});
			}

			
		}
		bool eraseHub();
	}
}