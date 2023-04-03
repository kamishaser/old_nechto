#pragma once
#include "connecter.h"
#include "creator.h"

#include "hubPtr.h"
#include "groupPtr.h"
#include "connectionPointer.h"

namespace nechto
{
	//����� ������� ��� �������� �����
	class hubManager
	{
	public:
		static void insertHub(existing<portPointer> begin, int quantity = 1)
		{
			nodePtr previous = begin.getHPPair().hub;
			nodePtr next = previous.hub();
			ui32 hubNumber = begin.getHubNumber();
			//���� �������� � ������� �����
			for (int i = 0; i < quantity; ++i)
			{
				hubPtr hub = creator::createHub(0);
				hub.connect(previous, hubNumber + i);
				previous = hub;
			}
			if (!next.exist())
				return;
			//��������� �������
			hubPtr(next).connect(previous, hubNumber + quantity);
			portPointer ptr(begin.getPurpose());
			do
			{
				if (portPointerPtr::match(ptr.get()))
				{
					auto hpp = portPointerPtr(ptr.get()).getHPPair();
					if (hpp.getHubNumber() > hubNumber)
					{
						hpp.setHubNumber(hpp.getHubNumber() + quantity);
						portPointerPtr(ptr.get()).setHPPair(hpp);
					}
				}
			} while (ptr.stepForward());
		}
		static void insertHub(groupPointer begin, int quantity = 1)
		{
			hubPtr previous = begin.getHPPair().hub;
			hubPtr next = previous.hub();
			ui32 hubNumber = begin.getHubNumber();
			groupPtr group(begin.getPurpose());
			group.setSize(group.getNumberOfHubs() + quantity);
			//���� �������� � ������� �����
			for (int i = 0; i < quantity; ++i)
			{
				hubPtr hub = creator::createHub(1);
				hub.connect(previous, hubNumber + i);
				previous = hub;
				begin.goToNextHub();
			}
			//��������� �������
			hubPtr(next).connect(previous, hubNumber + quantity);
			if (next == begin.firstHub())
				return;

			portPointer ptr(begin.getPurpose());
			do
			{
				if (groupPointerPtr::match(ptr.get()))
				{
					auto hpp = groupPointerPtr(ptr.get()).getHPPair();
					if (hpp.getHubNumber() > hubNumber)
					{
						hpp.setHubNumber(hpp.getHubNumber() + quantity);
						groupPointerPtr(ptr.get()).setHPPair(hpp);
					}
				}
			} while (ptr.stepForward());
		}
		static bool eraseHub(portPointer& begin, int quantity)
		{
			if (!begin.exist())
				return false;
			return pEraser(begin, quantity).eraseWithNotification();
		}
		static bool eraseHub(groupPointer& begin, int quantity)
		{
			if (!begin.exist())
				return false;
			return gEraser(begin, quantity).eraseWithNotification();
		}

		static bool eraseHubWithNoNotificationPointers(
			portPointer& begin, int quantity)
		{
			if (!begin.exist())
				return false;
			return pEraser(begin, quantity).eraseWOnotification();
		}
		static bool eraseHubWithNoNotificationPointers(
			groupPointer& begin, int quantity)
		{
			if (!begin.exist())
				return false;
			return gEraser(begin, quantity).eraseWOnotification();
		}
	private:
		//���������� ��� �������������� ������� ���������
		struct pEraser
		{
			portPointer& begin;
			hubPtr hub;
			nodePtr next;
			existing<nodePtr> previous;
			int quantity;//���������� ��������� �����. ����� ���������� ���������
			pEraser(portPointer& b, int q)
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
				notifyPointers();
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
					hub.connect(previous, begin.getHubNumber());
				else
				{
					if (next.exist())
						hub.connect(previous, begin.getHubNumber());
					else
						previous.node()->hubPort = nullptr;
				}
			}
			//����������
			void notifyPointers()
			{
				//��������� �������
				int max = begin.getHubNumber();
				int min = max - quantity;
				if (quantity == 0)
					return;
				portPointer ptr(begin.getPurpose());
				do
				{
					if (portPointerPtr::match(ptr.get()))
					{
						portPointerPtr temp(ptr.get());
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
				} while (ptr);
			}
			
		};
		struct gEraser
		{
			groupPointer& begin;
			hubPtr hub;
			hubPtr next;
			hubPtr previous;
			int quantity;//���������� ��������� �����. ����� ���������� ���������
			gEraser(groupPointer& b, int q)
				:begin(b), hub(begin.hub),
				previous(hub.previous()), next(hub.hub()), quantity(q) {}

			bool eraseWOnotification()
			{
				if (begin.atFirstHub())
					return false;
				bool result = erase();
				close(result);
				groupPtr group(begin.getPurpose());
				group.setSize(group.getNumberOfHubs() - quantity);
				return result;
			}
			bool eraseWithNotification()
			{
				if (begin.atFirstHub())
					return false;
				bool result = erase();
				close(result);
				groupPtr group(begin.getPurpose());
				group.setSize(group.getNumberOfHubs() - quantity);
				notifyPointers();
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

				hub.connect(previous, begin.getHubNumber());
			}
			//����������
			void notifyPointers()
			{
				//��������� �������
				int max = begin.getHubNumber();
				int min = max - quantity;
				if (quantity == 0)
					return;
				portPointer ptr(begin.getPurpose());
				do
				{
					if (groupPointerPtr::match(ptr.get()))
					{
						groupPointerPtr temp(ptr.get());
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
				} while (ptr);
			}
			void notifyConnections()
			{
			
			}

		};
	};
}
