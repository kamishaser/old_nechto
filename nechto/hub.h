#pragma once
#include "node.h"
namespace nechto
{
	const nodePtr newNode();
	void disconnect(nodePtr v1, nodePtr v2);
	
	namespace hub
	{
		//��� ������ ��� ����������� �������� ��������� ������:
		//hubConnection: ��������� ���
		//data: ���� �������: ���������� ��� � �������� ����

		//������ ����� ��� � ��������� � ������� ����� ��������.
		inline nodePtr insert(nodePtr previous, nodePtr mainNode)
		{//��������� ��� � ������� ����� ��������

			const nodePtr hub = newNode(node::Hub);
			hub->setData(std::pair<nodePtr, nodePtr>(previous, mainNode));
			
			nodePtr next = previous->hubConnection;
			if (next.exist())
			{
				hub->hubConnection = next;
				next->setData(std::pair<nodePtr, nodePtr>(hub, mainNode));
			}
			return hub;
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
		//���������� ���
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
		//��������� ��� �� �������
		void hubDisconnect(nodePtr v1, i64 number)
		{
			v1->connection[number].exchange(nullNodePtr), getMain(v1));

		}
		void erase(nodePtr v1, nodePtr mainNode)
		{
			assert(typeCompare(v1, node::Hub));
			nodePtr previous = previous(v1);
			nodePtr next = v1->hubConnection;
			for (int i = 0; i < 4; ++i)
				hubDisconnect(v1, i);
			previous->hubConnection = next;
			if (next.exist())
				next->setData(std::pair<nodePtr, nodePtr>(previous, mainNode));
		}

		/*void fill(nodePtr v1, char subtype);
		void reset(nodePtr v1);
		void perform(nodePtr v1);
		bool check(nodePtr v1);
		void copy(nodePtr v1);*/
	}
}