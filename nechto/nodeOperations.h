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
	//сравнение типов
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
	//проверка наличия соединения
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
	//номер подключения v2 к v1 нод. -1, если соединение не найдено
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
	//создание
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
		case node::MathOperator:			//математический оператор
		case node::ConditionalBranching:	//if
			break;
		case node::ExternalFunction:		//функция, не являющаяся частью nechto
			externalFunction::initializeEmpty(v1);
			break;
		case node::Text:					//метка
			text::initialize(v1);
			break;
		case node::ExternalConnection:		//внешнее подключение
			externalConnection::intializeNode(v1);
			break;
		case node::Pointer:				//указатель на объект
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
	//операции с хабами

	
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//создание одностороннего соединения
	void NumConnect(nodePtr v1, nodePtr v2, ushort conNumber)
	{//безопасное добавление односторонней связи
		//осуществляется привязка v2 к v1 по номеру conNumber
		nodePtr temp = nullNodePtr;
		if (v1->connection[conNumber].compare_exchange_strong(temp, v2))
			return;
		v1->connection[conNumber] = v2;
		oneSideDisconnect(temp, v1);
	}//возвращает true, если связь добавить удалось и false в противном случае

	void HubConnect(nodePtr v1, nodePtr v2)
	{//добавление связи в первое попавшееся свободное место в хабе
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
	//создание двухстороннего соединения
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
	//разрыв соединения

	//односторонне отстоединяет v2 от v1 (v1 не будет знать о v2)
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
	// удаление
	void deleteNode(nodePtr v1)
	{
		assert(v1.exist());
		nodePtr vTemp = v1;
		switch (v1->getType())
		{
		case node::Hub:					//разветвитель
			assert(false);//хабы удаляются отдельно
			break;
		case node::Variable:
		case node::MathOperator:			//математический оператор
		case node::ConditionalBranching:	//if
			break;
		case node::ExternalFunction:		//функция, не являющаяся частью nechto
			externalFunction::reset(v1);
			break;
		case node::Text:					//метка
			text::reset(v1);
			break;
		case node::ExternalConnection:		//внешнее подключение
			externalConnection::resetNode(v1);
			break;
		case node::Pointer:				//указатель на объект
			break;
		case node::Array:
			array::reset(v1);
			break;
		default:
			break;
		}
		while (true)
		{//цикл удаления узла со всеми хабами
			vTemp->setData<void*>(nullptr);
			for (int i = 0; i < 4; i++)//разрыв соединения
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
	//список соединений
}