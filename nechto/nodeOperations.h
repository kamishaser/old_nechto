#pragma once
#include "node.h"

#include <utility>
#include <cassert>
#include <set>

#include "baseValueTypes.h"
#include "mathOperator.h"
#include "tag.h"
#include "pointer.h"
#include "externalFunction.h"
#include "hub.h"

namespace nechto
{
	//��������� �����
	bool typeCompare(nodePtr v1, char type);
	bool subtypeCompare(nodePtr v1, char subtype);
	bool typeSubtypeCompare(nodePtr v1, char type, char subtype);
	//�������� ������� ����������
	bool isHubExist(nodePtr v1);
	bool isNodeHasConnections(nodePtr v1);
	bool hasConnection(nodePtr v1, nodePtr v2);
	bool hasMultipleConnection(nodePtr v1);

	i64 getConnectionNumber(nodePtr v1, nodePtr v2);
	std::vector<nodePtr> allNodeConnections(nodePtr v1);
	//��������
	const nodePtr newNode();
	const nodePtr newNode(char type, char subtype = 0, size_t data = 0);
	
	//�������� �������������� ����������
	void NumConnect(nodePtr v1, nodePtr v2, ushort conNumber);
	void HubConnect(nodePtr v1, nodePtr v2);
	//�������� �������������� ���������
	void NumNumConnect(nodePtr v1, nodePtr v2, ushort number1, ushort number2);
	void NumHubConnect(nodePtr v1, nodePtr v2, ushort number1);
	void HubHubConnect(nodePtr v1, nodePtr v2);
	//������ ����������
	void oneSideDisconnect(nodePtr v1, nodePtr v2);
	void disconnect(nodePtr v1, nodePtr v2);
	void numDisconnect(nodePtr v1, i64 conNum);
	//����� ����
	void reset(nodePtr v1);
	void setTypeAndSubtype(nodePtr v1, char type, char subtype = 0);
	
	//��������
	void deleteNode(nodePtr v);
	



	/////////////////////////////////////////////////////////////////////////////////////////////////
	//��������� �����
	bool typeCompare(nodePtr v1, char type)
	{
		if (!v1.exist())
			return false;
		return (v1->getType() == type);
	}
	bool subtypeCompare(nodePtr v1, char subtype)
	{
		if (!v1.exist())
			return false;
		return (v1->getSubtype() == subtype);
	}
	bool typeSubtypeCompare(nodePtr v1, char type, char subtype)
	{
		if (!v1.exist())
			return false;
		if (v1->getType() != type)
			return false;
		if (v1->getSubtype() != subtype)
			return false;
		return true;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//�������� ������� ����������
	bool isHubExist(nodePtr v1)
	{
		assert(v1.exist());
		return v1->hubConnection.load().exist();
	}
	bool isNodeHasConnections(nodePtr v1)
	{
		assert(v1.exist());
		nodePtr hubIterator = v1;
		while (true)
		{
			for (int i = 0; i < 4; i++)
			{
				if (hubIterator->connection[i].load().exist())
					return true;
			}
			if (!hubIterator->hasHub())
				return false;
			hubIterator = hubIterator->hubConnection;
		}
	}
	//����� ����������� v2 � v1 ���. -1, ���� ���������� �� �������
	i64 getConnectionNumber(nodePtr v1, nodePtr v2)
	{
		assert(v1.exist());
		i64 number = 0;
		nodePtr hubIterator = v1;
		while (true)
		{
			for (int i = 0; i < 4; i++)
			{
				if (hubIterator->connection[i].load() == v2)
					return number;
				++number;
			}
			if (!hubIterator->hasHub())
				return -1;
			hubIterator = hubIterator->hubConnection;
		}
	}

	std::vector<nodePtr> allNodeConnections(nodePtr v1)
	{
		assert(v1.exist());
		nodePtr hubIterator = v1;
		std::vector<nodePtr> connections;
		while (true)
		{
			for (int i = 0; i < 4; i++)
			{
				connections.push_back(hubIterator->connection[i].load());
			}
			if (!hubIterator->hasHub())
				return connections;
			hubIterator = hubIterator->hubConnection;
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//��������
	const nodePtr newNode()
	{
		nodePtr v;
		nodePtr temp = nodeStorage::terminal.allocate();
		v = temp;
		return v;
	}
	const nodePtr newNode(char type, char subtype, size_t data)
	{
		nodePtr v;
		nodePtr temp = nodeStorage::terminal.allocate();
		v = temp;
		setTypeAndSubtype(v, type, subtype);
		v->setData(data);
		return v;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//�������� � ������

	
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//�������� �������������� ����������
	void NumConnect(nodePtr v1, nodePtr v2, ushort conNumber)
	{//���������� ���������� ������������� �����
		//�������������� �������� v2 � v1 �� ������ conNumber
		nodePtr temp = nullNodePtr;
		if (v1->connection[conNumber].compare_exchange_strong(temp, v2))
			return;
		v1->connection[conNumber] = v2;
		oneSideDisconnect(temp, v1);
	}//���������� true, ���� ����� �������� ������� � false � ��������� ������

	void HubConnect(nodePtr v1, nodePtr v2)
	{//���������� ����� � ������ ���������� ��������� ����� � ����
		assert(v1.exist() && v2.exist());

		if (!v1->hubConnection.load().exist())
			hub::add(v1, v1);
		nodePtr hubIterator = v1->hubConnection;
		nodePtr temp;
		while (true)
		{
			temp = nullNodePtr;
			if (hubIterator->connection[0].compare_exchange_strong(temp, v2))
				return;
			temp = nullNodePtr;
			if (hubIterator->connection[1].compare_exchange_strong(temp, v2))
				return;
			temp = nullNodePtr;
			if (hubIterator->connection[2].compare_exchange_strong(temp, v2))
				return;
			temp = nullNodePtr;
			if (hubIterator->connection[3].compare_exchange_strong(temp, v2))
				return;
			hub::add(hubIterator, v1);
			hubIterator = hubIterator->hubConnection;
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//�������� �������������� ����������
	void NumNumConnect(nodePtr v1, nodePtr v2, ushort number1, ushort number2)
	{
		assert(v1.exist() && v2.exist());
		NumConnect(v1, v2, number1);
		NumConnect(v2, v1, number2);
	}
	void NumHubConnect(nodePtr v1, nodePtr v2, ushort number1)
	{
		assert(v1.exist() && v2.exist());
		assert(v1->getType() != node::Hub);
		assert(v2->getType() != node::Hub);
		NumConnect(v1, v2, number1);
		HubConnect(v2, v1);
	}
	void HubHubConnect(nodePtr v1, nodePtr v2)
	{
		assert(v1.exist() && v2.exist());
		assert(v1->getType() != node::Hub);
		assert(v2->getType() != node::Hub);
		HubConnect(v1, v2);
		HubConnect(v2, v1);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	//������ ����������

	//������������ ������������ v2 �� v1 (v1 �� ����� ����� � v2)
	void oneSideDisconnect(nodePtr v1, nodePtr v2)
	{
		assert(v1.exist() && v2.exist());
		nodePtr temp = v2;
		while (true)
		{
			if (v1->connection[0].compare_exchange_strong(temp, nullNodePtr)) return;
			else temp = v2;
			if (v1->connection[1].compare_exchange_strong(temp, nullNodePtr)) return;
			else temp = v2;
			if (v1->connection[2].compare_exchange_strong(temp, nullNodePtr)) return;
			else temp = v2;
			if (v1->connection[3].compare_exchange_strong(temp, nullNodePtr)) return;
			else temp = v2;

			if (!(v1->hubConnection.load()))
				return;
			else
				v1 = v1->hubConnection;
		}
	}
	void disconnect(nodePtr v1, nodePtr v2)
	{
		if (!v1.exist() || !v2.exist())
			return;
		oneSideDisconnect(v1, v2);
		oneSideDisconnect(v2, v1);
	}
	void numDisconnect(nodePtr v1, i64 conNum)
	{
		//���������� ��� �������� ���� �� �������
		assert(v1.exist());
		i64 hubNumber = conNum >> 2;

		nodePtr hubIterator = v1;
		for (i64 i = 0; i < (conNum >> 2); ++i)
		{
			if (!v1->hasHub())
				return;
			hubIterator = hubIterator->hubConnection;
		}
		oneSideDisconnect(hubIterator->connection[conNum & 3ll].exchange(nullNodePtr), v1);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//����� ����
	//����� �������������� ������
	void reset(nodePtr v1)
	{
		switch (v1->getType())
		{
		case node::Tag:
			tag::deleteData(v1);
			break;
		default:
			break;
		}
	}
	//��������� ���� � �������
	void setTypeAndSubtype(nodePtr v1, char type, char subtype)
	{
		reset(v1);
		v1->type = type;
		v1->subtype = subtype;

		switch (v1->getType())
		{
		case node::Tag:
		case node::ExternalFunction:
			v1->setData(nullptr);
			break;
		case node::Variable:
			switch (v1->getSubtype())
			{
			case baseValueType::I64:
				v1->setData<i64>(0);
				break;
			case baseValueType::F64:
				v1->setData<f64>(0);
				break;
			default:
				break;
			}
		default:
			break;
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// ��������
	void deleteNode(nodePtr v1)
	{
		assert(v1.exist());
		nodePtr vTemp = v1;
		reset(v1);
		while (true)
		{//���� �������� ���� �� ����� ������
			setTypeAndSubtype(vTemp, node::Error, 0);
			vTemp->setData<void*>(nullptr);
			for (int i = 0; i < 4; i++)//������ ����������
				if (vTemp->connection[i].load().exist())
				{
					oneSideDisconnect(vTemp->connection[i], v1);
					vTemp->connection[i] = nullNodePtr;
				}
			if (!vTemp->hubConnection.load().exist())
				break;

			nodePtr vHub = vTemp->hubConnection;
			nodeStorage::terminal.deallocate(vTemp);
			vTemp = vHub;
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	//������ ����������
}