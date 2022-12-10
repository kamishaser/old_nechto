#pragma once
#include "connecter.h"
#include "creator.h"

#include "hubPtr.h"
#include "groupPtr.h"
#include "connectionIterator.h"

namespace nechto
{
	//набор функция для удаления хабов
	class hubEraser
	{
	public:
		static void insertHub(existing<portIterator> begin, int quantity = 1)
		{
			nodePtr previous = begin.getHPPair().hub;
			nodePtr next = previous.hub();
			ui32 hubNumber = begin.getHubNumber();
			//цикл создания и вставки хабов
			for (int i = 0; i < quantity; ++i)
			{
				hubPtr hub = creator::createHub();
				hub.connect(previous);
				previous = hub;
			}
			if (!next.exist())
				return;
			//замыкание цепочки
			hubPtr(next).connect(previous);
			portIterator iter(begin.getPurpose());
			do
			{
				if (portIteratorPtr::match(iter.get()))
				{
					auto hpp = portIteratorPtr(iter.get()).getHPPair();
					if (hpp.getHubNumber() > hubNumber)
					{
						hpp.setHubNumber(hpp.getHubNumber() + quantity);
						portIteratorPtr(iter.get()).setHPPair(hpp);
					}
				}
			} while (iter.stepForward());
		}
		static void insertHub(existing<groupIterator> begin, int quantity = 1)
		{
			hubPtr previous = begin.getHPPair().hub;
			hubPtr next = previous.hub();
			ui32 hubNumber = begin.getHubNumber();
			//цикл создания и вставки хабов
			for (int i = 0; i < quantity; ++i)
			{
				hubPtr hub = creator::createHub();
				hub.connect(previous);
				previous = hub;
			}
			//замыкание цепочки
			hubPtr(next).connect(previous);
			if (next == begin.firstHub())
				return;

			portIterator iter(begin.getPurpose());
			do
			{
				if (groupIteratorPtr::match(iter.get()))
				{
					auto hpp = groupIteratorPtr(iter.get()).getHPPair();
					if (hpp.getHubNumber() > hubNumber)
					{
						hpp.setHubNumber(hpp.getHubNumber() + quantity);
						groupIteratorPtr(iter.get()).setHPPair(hpp);
					}
				}
			} while (iter.stepForward());
		}
		static bool eraseHub(existing<portIterator>& begin, int quantity)
		{
			return pEraser(begin, quantity).eraseWithNotification();
		}
		static bool eraseHub(existing<groupIterator>& begin, int quantity);
	private:
		static bool eraseHubWithNoNotificationIterators( // without - WOnotif
			existing<portIterator>& begin, int quantity);
		static bool eraseHubWithNoNotificationIterators(
			existing<groupIterator>& begin, int quantity);
		//структурки для взаимодействия функций удавления
		struct pEraser
		{
			existing<portIterator>& begin;
			hubPtr hub;
			nodePtr next;
			existing<nodePtr> previous;
			int quantity;//количество удаляемых хабов. Позже становится счётчиком
			pEraser(existing<portIterator>& b, int q)
				:begin(b), hub(begin.hub),
				previous(hub.previous()), next(hub.hub()), quantity(q) {}

			bool eraseWOnotification()
			{
				if (begin.getHPPair().hub.type() != nodeT::Hub)
					return false;
				bool result = erase();
				close(result);
				return result;
			}
			bool eraseWithNotification()
			{
				if (begin.getHPPair().hub.type() != nodeT::Hub)
					return false;
				bool result = erase();
				close(result);
				notify();
				return result;
			}
		private:
			//удавить
			bool erase()
			{
				int max = quantity;
				quantity = 0;
				while (quantity < max)
				{
					next = hub.hub();
					if (!hub.empty())//нельзя удалять не пустые хабы
					{
						return false;
					}
					creator::deleteHub(hub);
					++quantity;
					if (!next.exist())//цепочка прервана
						return true;
					hub = hubPtr(next);
				}
				return true;
			}
			//замкнуть + переместить итератор
			void close(bool result)
			{
				//установка итератора begin в следующий после удалённых хаб
				begin.hub = hub;
				begin.setGlobalPos(begin.getGlobalPos() + (quantity << 2));

				if (!result)//если завершено с ошибкой
					hub.connect(previous);
				else
				{
					if (next.exist())
						hub.connect(previous);
					else
						previous.node()->hubPort = nullptr;
				}
			}
			//оповестить
			void notify()
			{
				//замыкание цепочки
				int max = begin.getHubNumber();
				int min = max - quantity;
				if (quantity == 0)
					return;
				portIterator iter(begin.getPurpose());
				do
				{
					if (portIteratorPtr::match(iter.get()))
					{
						portIteratorPtr temp(iter.get());
						if (temp.connection(0) != begin.getPurpose())
							continue;
						auto hpp = temp.getHPPair();
						int hubNumber = hpp.getHubNumber();
						if (hubNumber < min)
							continue;//следующая итерация
						if (hubNumber < max)
							hpp = hubPosPair();
						else
							hpp.setHubNumber(hubNumber - quantity);
						temp.setHPPair(hpp);
					}
				} while (iter);
			}

		};
		struct eraserGData
		{
			existing<groupIterator>& begin;
			hubPtr hub;
			hubPtr next;
			hubPtr previous;
			int quantity;
			eraserGData(existing<groupIterator>& b, int q)
				:begin(b), hub(begin.getHPPair().hub),
				previous(hub.previous()), next(hub.hub()), quantity(q) {}
		private:
			//удавить
			bool erase()
				//замкнуть
			bool close();
			//оповестить
			void notify();

		};
	public:
		//удаление хаба из группы
		static bool eraseHub(existing<groupIterator>& begin, int quantity)
		{
			hubPtr hub(begin.getHPPair().hub);
			existing<nodePtr> previous = hub.previous();//предыдущий хаб
			hubPtr next(hub.hub());//следующий хаб
			

			bool error = false;
			int i = 0;//счётчик удалённых хабов
			while (i < quantity)
			{
				next = hub.hub();
				if (!hub.empty())//нельзя удалять не пустые хабы
				{
					error = true;
					break;
				}
				creator::deleteHub(hub);
				++i;
				hub = next;
				if (hub == begin.group().firstGroupHub())//цепочка прервана
					break;
			}
			//установка итератора begin в следующий после удалённых хаб
			begin.hub = hub;
			begin.setGlobalPos(begin.getGlobalPos() + (i << 2));
			//замыкание цепочки
			hub.connect(previous);
			//оповещение всех итераторов
			int min = begin.getHubNumber();//опасно!! в begin старая позиция!!
			int max = min + i;
			portIterator iter(begin.getPurpose());
			do
			{
				if (groupIteratorPtr::match(iter.get()))
				{
					groupIteratorPtr temp(iter.get());
					auto hpp = temp.getHPPair();
					int hubNumber = hpp.getHubNumber();
					if (hubNumber < min)
						continue;//следующая итерация
					if (hubNumber < max)
						hpp = hubPosPair();
					else
						hpp.setHubNumber(hubNumber - i);
					temp.setHPPair(hpp);
				}
			} while (iter);
			return !error;
		}
	};
}
