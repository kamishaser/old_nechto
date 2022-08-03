#pragma once
#include "node.h"
namespace nechto
{
	const nodePtr newNode();
	void disconnect(nodePtr v1, nodePtr v2);
	
	//��� ������ ��� ����������� �������� ��������� ������:
		//hubConnection: ��������� ���
		//data: ���� �������: ���������� ��� � �������� ����.
	namespace hub
	{
		//������ ����� ��� � ��������� � ������� ����� ��������
		inline nodePtr insert(nodePtr previous, nodePtr mainNode)
		{

			nodePtr hub = newNode(node::Hub);
			hub->setData(std::pair<nodePtr, nodePtr>(previous, mainNode));
			
			nodePtr next = previous->hubConnection;
			previous->hubConnection = hub;
			if (next.exist())
			{
				hub->hubConnection = next;
				next->setData(std::pair<nodePtr, nodePtr>(hub, mainNode));
			}
			return hub;
		}
		inline nodePtr pushBack(nodePtr chainEnd, nodePtr mainNode)
		{
			assert(!chainEnd->hasHub());
			nodePtr hub = newNode(node::Hub);
			hub->setData(std::pair<nodePtr, nodePtr>(chainEnd, mainNode));
			chainEnd->hubConnection = hub;
			return hub
		}
		const nodePtr getMain(const nodePtr hub)
		{
			assert(hub->getType() == node::Hub);
			return hub->getData<std::pair<nodePtr, nodePtr>>().second;
		}
		//����� ������� ����� (!��������! �� �������� � ���������)
		nodePtr chainEnd(nodePtr v1)
		{
			nodePtr iter = v1;
			nodePtr next = iter->hubConnection;
			while (next.exist())
			{
				nodePtr iter = next;
				nodePtr next = iter->hubConnection;
			}
			return iter;
		}
		//��������� ���� � �������
		nodePtr next(nodePtr v1)
		{
			nodePtr nextH = v1->hubConnection;
			if (nextH.exist())
				return nextH;
			else
				return v1->getData<std::pair<nodePtr, nodePtr>>().second;
		}
		//���������� ���� � �������
		nodePtr previous(nodePtr v1)
		{
			if (v1->getType() == node::Hub)
				return v1->getData<std::pair<nodePtr, nodePtr>>().first;
			//���� v1 �� ���, �� ��� �� �������� ������ � ��������� ����
			//������� ������� � ����� �������
			return chainEnd(v1);
		}
		//���������, ������ �� ���
		bool empty(nodePtr v1)
		{
			if (v1->hasConnection(0) || v1->hasConnection(1) ||
				v1->hasConnection(2) || v1->hasConnection(3))
				return false;
			return true;
		}
		//��������� ��� �� �������. ��� ������� ���������� � ���� 2-�������� ���������
		void erase(nodePtr v1, nodePtr mainNode)
		{
			assert(typeCompare(v1, node::Hub));
			nodePtr previous = previous(v1);
			nodePtr next = v1->hubConnection;
			for (int i = 0; i < 4; ++i)
			{
				nodePtr con = v1->connection[i].exchange(nullNodePtr);
				if(con.exist())
					nechto::oneSideDisconnect(con, mainNode);
			}
			previous->hubConnection = next;
			if (next.exist())
				next->setData(std::pair<nodePtr, nodePtr>(previous, mainNode));
			deleteNode(v1);
		}
		void deleteChain(nodePtr v1)
		{

		}

		/*void fill(nodePtr v1, char subtype);
		void reset(nodePtr v1);
		void perform(nodePtr v1);
		bool check(nodePtr v1);
		void copy(nodePtr v1);*/

	}
}