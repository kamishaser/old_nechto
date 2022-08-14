#pragma once
#include "node.h"
namespace nechto
{
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
		//������� ��� ���������� ����
		void deleteAllConnectionsInHub(nodePtr v1, nodePtr mainNode)
		{
			for (int i = 0; i < 4; ++i)
			{
				nodePtr con = v1->connection[i].exchange(nullNodePtr);
				if (con.exist())
					nechto::oneSideDisconnect(con, mainNode);
			}
		}
		//������� ���
		void deleteHub(nodePtr v1, nodePtr mainNode)
		{
			assert(v1->getType() == node::Hub);
			deleteAllConnectionsInHub(v1, mainNode);
		}
		//��������� ��� �� �������. ��� ������� ���������� � ���� 2-��������� ���������� ���������
		void erase(nodePtr v1, nodePtr mainNode)
		{
			assert(typeCompare(v1, node::Hub));
			nodePtr prev = previous(v1);
			nodePtr next = v1->hubConnection;
			prev->hubConnection = next;
			if (next.exist())
				next->setData(std::pair<nodePtr, nodePtr>(prev, mainNode));
			deleteHub(v1, mainNode);
		}
		bool checkChain(nodePtr v1)
		{
			nodePtr hubIter = v1;
			while (true)
			{
				nodePtr next = hubIter->hubConnection;
				if (!next.exist())
					return true;
				if (next->getType() != node::Hub)
					return false;
				auto data = next->getData<std::pair<nodePtr, nodePtr>>();
				if (data.first != hubIter || data.second != v1)
					return false;
				hubIter = next;
				
			}
		}
		//void compress(nodePtr v1)
		//{
		//	assert(v1.exist());
		//	assert(typeCompare(v1, node::Group));
		//	connectionIterator pullIter(v1);
		//	connectionIterator pushIter(v1);
		//	if (!v1->hasHub())
		//		return;
		//	pullIter.GoToNextHub();
		//	pushIter.GoToNextHub();
		//	do
		//	{
		//		nodePtr temp = *pullIter;
		//		*pullIter = nullNodePtr;//���� ���������� ��� �����������
		//		if (temp.exist())
		//		{
		//			*pushIter = temp;
		//			pushIter.stepForward();
		//		}
		//	} while (pullIter.stepForward());
		//	if (pushIter.pos() != 0)
		//		pushIter.stepForward();
		//	while (pushIter.currentHub != v1->connection[0])
		//	{
		//		assert(hub::empty(pushIter.currentHub));
		//		pushIter.eraseHub();
		//	}
		//}

	}
}