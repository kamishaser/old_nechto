#pragma once
#include "node.h"

#include <utility>
#include <cassert>
#include <set>

#include "mathOperator.h"
#include "text.h"
#include "pointer.h"
#include "externalFunction.h"
#include "connectionIterator.h"
#include "externalConnection.h"
#include "array.h"
#include "hub.h"

namespace nechto
{
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
	const nodePtr newNode(char type, char subtype)
	{
		nodePtr v1;
		nodePtr temp = nodeStorage::terminal.allocate();
		v1 = temp;
		v1->type = type;
		v1->subtype = subtype;

		switch (v1->getType())
		{
		case node::Variable:
		case node::MathOperator:			//�������������� ��������
		case node::ConditionalBranching:	//if
			break;
		case node::ExternalFunction:		//�������, �� ���������� ������ nechto
			externalFunction::initializeEmpty(v1);
			break;
		case node::Text:					//�����
			text::initialize(v1);
			break;
		case node::ExternalConnection:		//������� �����������
			externalConnection::intializeNode(v1);
			break;
		case node::Pointer:				//��������� �� ������
			pointer::initializeEmpty(v1);
			break;
		case node::Array:
			array::initializeEmpty(v1);
			break;
		default:
			break;
		}
		return v1;
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
			hub::pushBack(v1, v1);
		nodePtr hubIterator = v1->hubConnection;
		while (true)
		{
			for (int i = 0; i < 4; ++i)
				if (!hubIterator->hasConnection(i))
				{
					hubIterator->connection[i] = v2;
					return;
				}
			nodePtr next = hubIterator->hubConnection;
			hubIterator = (next.exist()) ? next : hub::pushBack(hubIterator, v1);
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

	void IterHubConnect(hubIterator i1, nodePtr v2)
	{
		nodePtr oldCon1 = i1.oneSideConnect(v2);
		if (oldCon1.exist())
			oneSideDisconnect(oldCon1, i1.mainNode);
		HubConnect(v2, i1.mainNode);
	}
	void IterIterConnect(hubIterator i1, hubIterator i2)
	{
		nodePtr oldCon1 = i1.oneSideConnect(i2.mainNode);
		nodePtr oldCon2 = i2.oneSideConnect(i1.mainNode);
		if (oldCon1.exist())
			oneSideDisconnect(oldCon1, i1.mainNode);
		if (oldCon2.exist())
			oneSideDisconnect(oldCon2, i2.mainNode);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	//������ ����������

	//������������ ������������ v2 �� v1 (v1 �� ����� ����� � v2)
	void oneSideDisconnect(nodePtr v1, nodePtr v2)
	{
		assert(v1.exist() && v2.exist());
		connectionIterator i1(v1);
		do
		{
			if (i1.get() == v2)
				i1.oneSideConnect(nullNodePtr);
		} while (i1.stepForward());
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
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// ��������
	void deleteNode(nodePtr v1)
	{
		assert(v1.exist());
		nodePtr vTemp = v1;
		switch (v1->getType())
		{
		case node::Hub:					//������������
			assert(false);//���� ��������� ��������
			break;
		case node::Variable:
		case node::MathOperator:			//�������������� ��������
		case node::ConditionalBranching:	//if
			break;
		case node::ExternalFunction:		//�������, �� ���������� ������ nechto
			externalFunction::reset(v1);
			break;
		case node::Text:					//�����
			text::reset(v1);
			break;
		case node::ExternalConnection:		//������� �����������
			externalConnection::resetNode(v1);
			break;
		case node::Pointer:				//��������� �� ������
			break;
		case node::Array:
			array::reset(v1);
			break;
		default:
			break;
		}
		while (true)
		{//���� �������� ���� �� ����� ������
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