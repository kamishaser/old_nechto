#pragma once
#include "node.h"

#include <utility>
#include <cassert>
#include <set>

namespace nechto
{
	//сравнение типов
	bool typeCompare(nodePtr v1, ushort type);
	//проверка наличия соединения
	bool isHubExist(nodePtr v1);
	bool isNodeHasConnections(nodePtr v1);
	bool hasConnection(nodePtr v1, nodePtr v2);
	bool hasMultipleConnection(nodePtr v1);
	//создание
	const nodePtr newNode();
	const nodePtr newNode(ushort type, ushort subtype = 0, size_t data = 0);
	
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
	//удаление
	void deleteNode(nodePtr v);
	//список соединений
	std::set<nodePtr>&& allNodeConnactions(nodePtr v1);
	
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//сравнение типов
	bool typeCompare(nodePtr v1, ushort type)
	{
		return v1->type.load() == type;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//проверка наличия соединения
	bool isHubExist(nodePtr v1)
	{
		return v1->hubConnection.load().exist();
	}
	bool isNodeHasConnections(nodePtr v1)
	{
		for (int i = 0; i < 4; i++)
			if (v1->hasConnection(i))
				return true;
		if (isHubExist(v1)) return true;
		return false;
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
		v->type = type;
		v->subtype = subtype;
		v->data = data;
		return v;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//операции с хабами

	void addHub(nodePtr v1)
	{//добавление хаба к элементу
		nodePtr hub = newNode();
		hub->type.store(node::Hub);
		hub->type.store(0);
		hub->setData<nodePtr>(v1);
		//адресс расширяемого элемента
		nodePtr temp = nullNodePtr;//ввиду того, что compare_excha
		if (!v1->hubConnection.compare_exchange_strong(temp, hub))
			deleteNode(hub);
		//если присоединить хаб не удалось, значит он уже есть
	}
	const nodePtr getHubParrent(const nodePtr hub)
	{
		assert(hub->type.load() == node::Hub);
		return hub->getData<nodePtr>();
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
		assert(v1 != nullNodePtr && v2 != nullNodePtr);
		if (!v1->hubConnection.load().exist())
			addHub(v1);
		v1=v1->hubConnection;

		nodePtr temp;
		while (true)
		{
			temp = nullNodePtr;
			if (v1->connection[0].compare_exchange_strong(temp, v2))
				return;
			temp = nullNodePtr;
			if (v1->connection[1].compare_exchange_strong(temp, v2))
				return;
			temp = nullNodePtr;
			if (v1->connection[2].compare_exchange_strong(temp, v2))
				return;
			temp = nullNodePtr;
			if (v1->connection[3].compare_exchange_strong(temp, v2))
				return;
			addHub(v1);
			v1 = v1->hubConnection;
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//создание двухстороннего соединения
	void NumHubConnect(nodePtr v1, nodePtr v2, ushort number1)
	{
		assert(v1 != nullNodePtr && v2 != nullNodePtr);
		assert(v1->type != node::Hub);
		assert(v2->type != node::Hub);
		NumConnect(v1, v2, number1);
		HubConnect(v2, v1);
	}
	void HubHubConnect(nodePtr v1, nodePtr v2)
	{
		assert(v1 != nullNodePtr && v2 != nullNodePtr);
		assert(v1->type != node::Hub);
		assert(v2->type != node::Hub);
		HubConnect(v1, v2);
		HubConnect(v2, v1);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	//разрыв соединения

	void oneSideDisconnect(nodePtr v1, nodePtr v2)
	{//односторонне отстоединяет v2 от v1 (v1 не будет знать о v2)
		assert(v1 != nullNodePtr && v2 != nullNodePtr);
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
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// удаление
	void deleteNode(nodePtr v1)
	{
		assert(!isNodeHasConnections(v1));
		nodePtr vTemp = v1;
		while (true)
		{//цикл удаления узла со всеми хабами
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
	std::set<nodePtr>&& allNodeConnactions(nodePtr v1)
	{
		std::set<nodePtr> nodeSet;
		while (true)
		{
			for (int i = 0; i < 4; i++)
			{
				if (v1->hasConnection(i))
					nodeSet.emplace(v1->connection[i]);
			}
			if (!v1->hasHub())
				break;
			v1 = v1->hubConnection;
		}
		return std::move(nodeSet);
	}
}