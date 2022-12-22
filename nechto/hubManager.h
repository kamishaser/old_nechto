#pragma once
#include "connecter.h"
#include "creator.h"

#include "hubPtr.h"
#include "groupPtr.h"
#include "connectionIterator.h"

namespace nechto
{
	//����� ������� ��� �������� �����
	class hubManager
	{
	public:
		static void insertHub(existing<portIterator> begin, int quantity = 1)
		{
			nodePtr previous = begin.getHPPair().hub;
			nodePtr next = previous.hub();
			ui32 hubNumber = begin.getHubNumber();
			//���� �������� � ������� �����
			for (int i = 0; i < quantity; ++i)
			{
				hubPtr hub = creator::createHub(0);
				hub.connect(previous);
				previous = hub;
			}
			if (!next.exist())
				return;
			//��������� �������
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
			//���� �������� � ������� �����
			for (int i = 0; i < quantity; ++i)
			{
				hubPtr hub = creator::createHub(1);
				hub.connect(previous);
				previous = hub;
			}
			//��������� �������
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
		static bool eraseHub(portIterator& begin, int quantity)
		{
			if (!begin.exist())
				return false;
			return pEraser(begin, quantity).eraseWithNotification();
		}
		static bool eraseHub(groupIterator& begin, int quantity)
		{
			if (!begin.exist())
				return false;
			return gEraser(begin, quantity).eraseWithNotification();
		}

		static bool eraseHubWithNoNotificationIterators(
			portIterator& begin, int quantity)
		{
			if (!begin.exist())
				return false;
			return pEraser(begin, quantity).eraseWOnotification();
		}
		static bool eraseHubWithNoNotificationIterators(
			groupIterator& begin, int quantity)
		{
			if (!begin.exist())
				return false;
			return gEraser(begin, quantity).eraseWOnotification();
		}
	private:
		//���������� ��� �������������� ������� ���������
		struct pEraser
		{
			portIterator& begin;
			hubPtr hub;
			nodePtr next;
			existing<nodePtr> previous;
			int quantity;//���������� ��������� �����. ����� ���������� ���������
			pEraser(portIterator& b, int q)
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
			//�������
			bool erase()
			{
				int max = quantity;
				quantity = 0;
				while (quantity < max)
				{
					next = hub.hub();
					if (!hub.empty())//������ ������� �� ������ ����
					{
						return false;
					}
					creator::deleteHub(hub);
					++quantity;
					if (!next.exist())//������� ��������
						return true;
					hub = hubPtr(next);
				}
				return true;
			}
			//�������� + ����������� ��������
			void close(bool result)
			{
				//��������� ��������� begin � ��������� ����� �������� ���
				begin.hub = hub;
				begin.setGlobalPos(begin.getGlobalPos() + (quantity << 2));

				if (!result)//���� ��������� � �������
					hub.connect(previous);
				else
				{
					if (next.exist())
						hub.connect(previous);
					else
						previous.node()->hubPort = nullptr;
				}
			}
			//����������
			void notify()
			{
				//��������� �������
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
							continue;//��������� ��������
						if (hubNumber < max)
							hpp = hubPosPair();
						else
							hpp.setHubNumber(hubNumber - quantity);
						temp.setHPPair(hpp);
					}
				} while (iter);
			}
		};
		struct gEraser
		{
			groupIterator& begin;
			hubPtr hub;
			hubPtr next;
			hubPtr previous;
			int quantity;//���������� ��������� �����. ����� ���������� ���������
			gEraser(groupIterator& b, int q)
				:begin(b), hub(begin.hub),
				previous(hub.previous()), next(hub.hub()), quantity(q) {}

			bool eraseWOnotification()
			{
				if (begin.atFirstHub())
					return false;
				bool result = erase();
				close(result);
				return result;
			}
			bool eraseWithNotification()
			{
				if (begin.atFirstHub())
					return false;
				bool result = erase();
				close(result);
				notify();
				return result;
			}
		private:
			//�������
			bool erase()
			{
				int max = quantity;
				quantity = 0;
				while (quantity < max)
				{
					next = hub.hub();
					if (!hub.empty())//������ ������� �� ������ ����
					{
						return false;
					}
					creator::deleteHub(hub);
					++quantity;
					hub = next;
					if (hub == begin.group().firstGroupHub())//������� ��������
						return true;
				}
				return true;
			}
			//�������� + ����������� ��������
			void close(bool result)
			{
				//��������� ��������� begin � ��������� ����� �������� ���
				begin.hub = hub;
				begin.setGlobalPos(begin.getGlobalPos() + (quantity << 2));

				hub.connect(previous);
			}
			//����������
			void notify()
			{
				//��������� �������
				int max = begin.getHubNumber();
				int min = max - quantity;
				if (quantity == 0)
					return;
				portIterator iter(begin.getPurpose());
				do
				{
					if (groupIteratorPtr::match(iter.get()))
					{
						groupIteratorPtr temp(iter.get());
						if (temp.connection(0) != begin.getPurpose())
							continue;
						auto hpp = temp.getHPPair();
						int hubNumber = hpp.getHubNumber();
						if (hubNumber < min)
							continue;//��������� ��������
						if (hubNumber < max)
							hpp = hubPosPair();
						else
							hpp.setHubNumber(hubNumber - quantity);
						temp.setHPPair(hpp);
					}
				} while (iter);
			}

		};
	};
}
