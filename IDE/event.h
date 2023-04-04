#pragma once
#include "ideStructurePack.h"
#include "connecter.h"
#include "portSearch.h"

namespace nechto::ide
{
	
	i64 numberOfNewEvents(nodePtr subscription)
	{
		return groupOperations::numberOfMembers(
			groupPtr(sPack::eventSubscription::nonHandledEvent / subscription));
	}
	nodePtr getEventGyIter(groupPointer eventIter)
	{
		return "3"_np / eventIter.get();
	}
	//проверка, соответствует ли событие подписке по типу
	bool eventTypeRespondSubscription(nodePtr event, nodePtr subscription)
	{
		nodePtr eventType = sPack::event::type / "3"_np / event;
		nodePtr eventSubscriptionType = 
			sPack::eventSubscription::type / "3"_np / subscription;
		if (+eventSubscriptionType && 
			eventType != eventSubscriptionType)
			return false;
		return true;
	}
	//проверка, соответствует ли событие подписке по источнику
	bool eventSourceRespondSubscription(nodePtr event, nodePtr subscription)
	{
		nodePtr eventSource = sPack::event::source / "3"_np / event;
		nodePtr eventSubscriptionSource =
			sPack::eventSubscription::source / "3"_np / subscription;
		if (+eventSubscriptionSource &&
			eventSource != eventSubscriptionSource)
			return false;
		return true;
	}
	//проверка, оповещена ли подписка о данном событии
	bool isSubscriptionNotified(nodePtr subscription, nodePtr event)
	{

		return 
			lastConnectedGroupPort(
			sPack::eventSubscription::nonHandledEvent / subscription).get() ==
			sPack::event::subscriptionGroup / event;
	}
	//оповещение подписки о событии
	void notifySubscriptionAboutEvent(nodePtr subscription, nodePtr event)
	{
		if(!isSubscriptionNotified(subscription, event))
			connecter::connect(
				backGroupPort(sPack::eventSubscription::nonHandledEvent / subscription),
				backGroupPort(sPack::event::subscriptionGroup / event));
	}
	//оповещение подписок по типу о событии
	void notifySubscriptionsByTypeAboutEvent(nodePtr type, nodePtr event)
	{
		groupPointer gi(sPack::eventType::subscriptionGroup / type);
		do
		{
			nodePtr subscription = "3"_np / gi.get();
			if (!(+subscription))
				continue;
			if(eventSourceRespondSubscription(event, subscription))
				notifySubscriptionAboutEvent(subscription, event);
		} while (gi.stepForward());
	}
	//оповещение подписок по источнику о событии
	void notifySubscriptionsBySourceAboutEvent(nodePtr source, nodePtr event)
	{
		groupPointer gi(sPack::eventSource::subscriptionGroup / source);
		do
		{
			nodePtr subscription = "3"_np / gi.get();
			if (!(+subscription))
				continue;
			if (eventTypeRespondSubscription(event, subscription))
				notifySubscriptionAboutEvent(subscription, event);
		} while (gi.stepForward());
	}
	i64 numberOfCreatedEvents = 0;//количество произошдших событий
	//создать, зарегистрировать событие, и сообщить о нём слушателям
	nodePtr addEvent(nodePtr storage, nodePtr source/*источник*/, nodePtr type, nodePtr content = nullptr)
	{
		nodePtr event = fabricate(sPack::event::getPlan());
		if (storage.exist())
			PointerNumConnect(backGroupPort(storage),
				event, 3);
		assert(source.exist());
		assert(type.exist());
		PointerNumConnect(
			backGroupPort(sPack::eventSource::eventGroup / source), event, 2);
		PointerNumConnect(
			backGroupPort(sPack::eventType::eventGroup / type),
			"0"_np / event, 0);
		notifySubscriptionsBySourceAboutEvent(source, event);
		notifySubscriptionsByTypeAboutEvent(type, event);
		if (+content)
			NumHubConnect("0"_np / event, content, 1);
		++numberOfCreatedEvents;
		return event;
	}
	void subscribeToSource(nodePtr subscription, nodePtr source)
	{
		PointerNumConnect(backGroupPort(sPack::eventSource::subscriptionGroup / source),
			sPack::eventSubscription::parameters / subscription, 0);
	}
	void subscribeToType(nodePtr subscription, nodePtr type)
	{
		PointerNumConnect(backGroupPort(sPack::eventType::subscriptionGroup / type),
			sPack::eventSubscription::parameters / subscription, 1);
	}
	void createHierarchicalTypeRelationship(nodePtr parentType, nodePtr childType)
	{
		connecter::connect(
			backGroupPort(sPack::eventType::parentGroup / childType),
			backGroupPort(sPack::eventType::childrenGroup / parentType));
	}
	void createHierarchicalSourceRelationship(nodePtr parentSource, nodePtr childSource)
	{
		connecter::connect(
			backGroupPort(sPack::eventSource::parentGroup / childSource),
			backGroupPort(sPack::eventSource::childrenGroup / parentSource));
	}
	nodePtr fabricateSubscription(nodePtr source, nodePtr type, nodePtr node = nullptr)
	{
		node = fabricate(sPack::eventSubscription::getPlan());
		if (source.exist())
			subscribeToSource(node, source);
		if (type.exist())
			subscribeToType(node, type);
		return node;
	}
	//перевести	событие в "обрабатываемые в данный момент"
	void moveToHandlingNow(nodePtr subscription, groupPointer eventIter)
	{
		pointer revPort = reversePort(eventIter);
		connecter::connect(revPort,
			backGroupPort(sPack::eventSubscription::handlingNowEvent / subscription));
	}
	//перевести	событие в "обработанные"
	void moveToHandled(nodePtr subscription, groupPointer eventIter)
	{
		pointer revPort = reversePort(eventIter);
		connecter::connect(revPort,
			backGroupPort(sPack::eventSubscription::handledEvent / subscription));
	}
	void deleteEvent(nodePtr event)
	{
		nodePtr data = "0"_np / event;
		nodePtr subscrGroup = "1"_np / event;
		creator::deleteNode(data);
		creator::deleteNode(subscrGroup);
		creator::deleteNode(event);
	}
	void deleteEvent(groupPointer eventIter)
	{
		deleteEvent("3"_np / eventIter.get());
	}
}
