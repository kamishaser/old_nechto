#pragma once
#include "node.h"

#include <utility>
#include <cassert>
#include <set>

#include "mathOperator.h"
#include "nodeOperator.h"
#include "text.h"
#include "pointer.h"
#include "method.h"
#include "connectionIterator.h"
#include "group.h"
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

	bool hasConnections(nodePtr v1)
	{
		assert(v1.exist());
		if (v1->getType() == node::Group)
		{
			assert(group::check(v1));
			groupIterator i1(v1);
			do
			{
				if (i1.get().exist())
				{
					return true;
				}
			} while (i1.stepForward());
		}
		connectionIterator i2(v1);
		do
		{
			if (i2.get().exist())
			{
				return true;
			}
		} while (i2.stepForward());
		return false;
	}
	bool hasConnection(nodePtr v1, nodePtr v2)
	{
		assert(v1.exist() && v2.exist());
		if (v1->getType() == node::Group)
		{
			assert(group::check(v1));
			groupIterator i1(v1);
			do
			{
				if (i1.get() == v2)
				{
					return true;
				}
			} while (i1.stepForward());
		}
		connectionIterator i2(v1);
		do
		{
			if (i2.get() == v2)
			{
				return true;
			}
		} while (i2.stepForward());
		return false;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//��������
	nodePtr newNode(char type, char subtype)
	{
		nodePtr v1 = nodeStorage::terminal.allocate();
		v1->type = type;
		v1->subtype = subtype;

		//��������! ��������� �� ����� ���� �������!!!
		switch (v1->getType())
		{
		case node::Variable:
		case node::MathOperator:			//�������������� ��������
		case node::ConditionalBranching:	//if
		case node::Method:
			break;
		case node::Text:					//�����
			text::initialize(v1);
			break;
		case node::ExternalObject:		//������� �����������
			externalObject::intializeNode(v1);
			break;
		case node::Group:
			group::initializeEmpty(v1);
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
		nodePtr temp = v1->connection[conNumber].exchange(v2);
		if (!temp.exist())
			return;
		oneSideDisconnect(temp, v1);
	}//���������� true, ���� ����� �������� ������� � false � ��������� ������

	namespace hub//������� ����������. ������������� ��� ���������� ������ �� h � cpp
	{
		//������ �������� ���� � ������� �����
		connectionIterator firstEmptyPort(nodePtr v1)
		{
			if (!v1->hasHub())
				hub::insertBack(v1, v1);
			nodePtr hubIter = v1->hubConnection;
			while (true)
			{
				for (int i = 0; i < 4; ++i)
					if (!hubIter->hasConnection(i))
					{
						return connectionIterator(hubIter, v1, i);
					}
				nodePtr next = hubIter->hubConnection;
				hubIter = (next.exist()) ? next : hub::insertBack(hubIter, v1);
			}
		}
	}
	void HubConnect(nodePtr v1, nodePtr v2)
	{//���������� ����� � ������ ���������� ��������� ����� � ����
		assert(v1.exist() && v2.exist());
		hub::firstEmptyPort(v1).oneSideConnect(v2);
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
		if (v1->getType() == node::Group)
		{
			assert(group::check(v1));
			groupIterator i1(v1);
			do
			{
				if (i1.get() == v2)
				{
					i1.oneSideDisconnect();
					return;
				}
			} while (i1.stepForward());
		}
		connectionIterator i2(v1);
		do
		{
			if (i2.get() == v2)
			{
				i2.oneSideDisconnect();
				return;
			}
		} while (i2.stepForward());
	}
	void disconnect(nodePtr v1, nodePtr v2)
	{
		if (!v1.exist() || !v2.exist())
			return;
		oneSideDisconnect(v1, v2);
		oneSideDisconnect(v2, v1);
	}
	void hubIterator::disconnect()
	{
		nechto::oneSideDisconnect(get(), mainNode);
		currentHub->connection[pos()] = nullNodePtr;
	}
	void numDisconnect(nodePtr v1, i64 number)
	{
		assert(v1.exist());
		nodePtr old = v1->connection[number];
		if (old.exist())
			oneSideDisconnect(old, v1);
		v1->connection[number] = nullNodePtr;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// ��������
	void deleteNode(nodePtr v1)
	{
		assert(v1.exist());
		assert(!typeCompare(v1, node::Deleted));//������ ������� ������

		switch (v1->getType())
		{
		case node::Hub:					//������������
			assert(false);//���� ��������� ��������
			break;
		case node::Variable:
		case node::MathOperator:			//�������������� ��������
		case node::ConditionalBranching:	//if
			break;
		case node::Text:					//�����
			text::reset(v1);
			break;
		case node::ExternalObject:		//������� �����������
			externalObject::resetNode(v1);
			break;
		case node::Pointer:				//��������� �� ������
			break;
		case node::Group:
			group::reset(v1);
			break;
		default:
			break;
		}
		
		/*���� �������� ���� �� ����� ������ 
		� �� ����� ������������ �� ���� �����������*/
		connectionIterator i1(v1);
		while (true)
		{
			nodePtr connection = i1.oneSideDisconnect();
			if (connection.exist())
			{
				//��� �������� ���� ���� ������� ��� ������������ ����������
				if (typeSubtypeCompare(connection, 
					node::Pointer, pointer::ConIter) &&
					(connection->getSubtype() == pointer::ConIter) &&
					(connection->connection[0] == v1))
				{
					connection->connection[0] = nullNodePtr;
					connection->setData<pointer::hubPosPair>(
						pointer::hubPosPair(nullNodePtr, 0));
					
				}
				else
					oneSideDisconnect(connection, v1);
			}
			if (i1.pos() == 3)
			{
				nodePtr vTemp = i1.currentHub;
				bool end = !i1.GoToNextHub();
				nodeStorage::terminal.deallocate(vTemp);
				i1.setLocalPos(0);
				if (end)
				{
					assert(typeCompare(i1.currentHub, node::Deleted));
					return;
				}
			}
			else
				i1.setLocalPos(i1.pos() + 1);
		} 
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	//������ ����������
}