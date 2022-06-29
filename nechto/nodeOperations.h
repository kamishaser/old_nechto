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

namespace nechto
{
	//сравнение типов
	bool typeCompare(nodePtr v1, char type);
	bool subtypeCompare(nodePtr v1, char subtype);
	bool typeSubtypeCompare(nodePtr v1, char type, char subtype);
	//проверка наличия соединения
	bool isHubExist(nodePtr v1);
	bool isNodeHasConnections(nodePtr v1);
	bool hasConnection(nodePtr v1, nodePtr v2);
	bool hasMultipleConnection(nodePtr v1);
	//создание
	const nodePtr newNode();
	const nodePtr newNode(char type, char subtype = 0, size_t data = 0);
	
	//операции с хабами
	void addHub(nodePtr v1);
	const nodePtr getHubParrent(const nodePtr hub);
	//создание одностороннего соединения
	void NumConnect(nodePtr v1, nodePtr v2, ushort conNumber);
	void HubConnect(nodePtr v1, nodePtr v2);
	//создание двухстороннего соединени
	void NumHubConnect(nodePtr v1, nodePtr v2, ushort number1);
	void HubHubConnect(nodePtr v1, nodePtr v2);
	//разрыв соединения
	void oneSideDisconnect(nodePtr v1, nodePtr v2);
	void disconnect(nodePtr v1, nodePtr v2);
	void numDisconnect(nodePtr v1, int64_t conNum);
	//смена типа
	void reset(nodePtr v1);
	void setTypeAndSubtype(nodePtr v1, char type, char subtype = 0);
	
	//удаление
	void deleteNode(nodePtr v);
	
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
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//создание
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
	//операции с хабами

	void addHub(nodePtr vertex, nodePtr lastHub)
	{//добавление хаба к элементу
		assert(vertex.exist());
		assert(lastHub.exist());

		nodePtr hub = newNode();
		setTypeAndSubtype(hub, node::Hub, 0);
		hub->setData(std::pair<nodePtr, nodePtr>(vertex, lastHub));
		//адресс расширяемого элемента
		nodePtr temp = nullNodePtr;//ввиду того, что compare_excha
		if (!lastHub->hubConnection.compare_exchange_strong(temp, hub))
			deleteNode(hub);
		//если присоединить хаб не удалось, значит он уже есть
	}
	const nodePtr getHubParrent(const nodePtr hub)
	{
		assert(hub->getType() == node::Hub);
		return hub->getData<std::pair<nodePtr, nodePtr>>().first;
	}
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
			addHub(v1, nullNodePtr);
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
			addHub(v1, hubIterator);
			hubIterator = hubIterator->hubConnection;
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//создание двухстороннего соединения
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
	//разрыв соединения

	//односторонне отстоединяет v2 от v1 (v1 не будет знать о v2)
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
	void numDisconnect(nodePtr v1, int64_t conNum)
	{
		assert(v1.exist());
		int64_t hubNumber = conNum >> 2;

		nodePtr hubIterator = v1;
		for (int64_t i = 0; i < (conNum >> 2); ++i)
		{
			if (!v1->hasHub())
				return;
			hubIterator = hubIterator->hubConnection;
		}
		oneSideDisconnect(hubIterator->connection[conNum & 3ll].exchange(nullNodePtr), v1);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//смена типа
	//сброс дополнительных данных
	void reset(nodePtr v1)
	{
		switch (v1->getType())
		{
		case node::Tag:
			tag::deleteData(v1);
			break;
		case node::Pointer:
			pointer::deletePointer(v1);
			break;
		default:
			break;
		}
	}
	//установка типа и подтипа
	void setTypeAndSubtype(nodePtr v1, char type, char subtype)
	{
		reset(v1);
		v1->type = type;
		v1->subtype = subtype;

		switch (v1->getType())
		{
		case node::Tag:
		case node::ExteralFunction:
			v1->setData(nullptr);
			break;
		case node::Variable:
			switch (v1->getSubtype())
			{
			case baseValueType::Int64:
				v1->setData<int64_t>(0);
				break;
			case baseValueType::Double:
				v1->setData<double>(0);
				break;
			default:
				break;
			}
		default:
			break;
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// удаление
	void deleteNode(nodePtr v1)
	{
		assert(v1.exist());
		nodePtr vTemp = v1;
		reset(v1);
		while (true)
		{//цикл удаления узла со всеми хабами
			setTypeAndSubtype(vTemp, node::Error, 0);
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