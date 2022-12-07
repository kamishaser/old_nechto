#pragma once
#include "connecter.h"
#include "creator.h"

#include "hubPtr.h"
#include "groupPtr.h"
#include "connectionIterator.h"

namespace nechto
{
	//����� ������� ��� �������� �����
	class hubEraser
	{
	public:
		//�������� ���� �� �������� �������
		static bool eraseHub(existing<portIterator> iter)
		{
			if (iter.getHPPair().hub.type() != nodeT::Hub)
				return false;
			hubPtr hub(existing<nodePtr>(iter.getHPPair().hub));
			nodePtr previous = hub.previous();//���������� ���
			nodePtr next = hub.hub();//��������� ���
			std::vector<portIteratorPtr> iterSet;//������ ���� ���������� �� ��� ����
			getAllPortIterators(iterSet, iter.getPurpose());
			if (!erase(iterSet, iter))
				return false;
			if (!next.exist())
				previous.node()->hubPort = nullptr;
			hubPtr(next).connect(previous);
			return true;
		}
		//�������� ���� �� ������
		static bool eraseHub(existing<groupIterator> iter)
		{
			hubPtr hub(existing<nodePtr>(iter.getHPPair().hub));
			nodePtr previous = hub.previous();//���������� ���
			hubPtr next = existing<nodePtr>(hub.hub());//��������� ���
			std::vector<groupIteratorPtr> iterSet;//������ ���� ���������� �� ��� ����

			getAllGroupIterators(iterSet, iter.getPurpose());
			if (!erase(iterSet, iter))
				return false;
			next.connect(previous);
			return true;
		}
		
		//�������� ���� �����
		static bool eraseHubChain(existing<portIterator> iter, 
			portIterator end = nullPortIterator)
		{
			if (iter.getHPPair().hub.type() != nodeT::Hub)
				return false;
			hubPtr hub(existing<nodePtr>(iter.getHPPair().hub));
			nodePtr previous = hub.previous();//���������� ���
			nodePtr next = hub.hub();//��������� ���
			std::vector<portIteratorPtr> iterSet;//������ ���� ���������� �� ��� ����
			getAllPortIterators(iterSet, iter.getPurpose());
			bool success = true;
			while (true)
			{
				if (!erase(iterSet, iter))
				{
					success = false;
					break;
				}
				
				if (!next.exist())//���� ����� ��������� ��� � �������
					break;
				if (hub == end.getHPPair().hub)//��� ��������� � end
					break;
				hub = hubPtr(existing<nodePtr>(next));
				next = hub.hub();
			}
			if (success)
			{
				if (!next.exist())
					previous.node()->hubPort = nullptr;
				hubPtr(next).connect(previous);
			}
			else
			{
				hub.connect(previous);
			}
			return success;
		}
		static bool eraseHubChain(existing<groupIterator> iter,
			groupIterator end = nullGroupIterator)
		{
			hubPtr hub(existing<nodePtr>(iter.getHPPair().hub));
			nodePtr previous = hub.previous();//���������� ���
			hubPtr next = existing<nodePtr>(hub.hub());//��������� ���
			groupPtr group = existing<nodePtr>(iter.getPurpose());

			std::vector<groupIteratorPtr> iterSet;//������ ���� ���������� �� ��� ����
			getAllGroupIterators(iterSet, iter.getPurpose());
			bool success = true;
			while (true)
			{
				if (!erase(iterSet, iter))
				{
					success = false;
					break;
				}

				if (next == group.firstGroupHub())//���� next - ������ ����
					break;
				if (hub == end.getHPPair().hub)//��� ��������� � end
					break;
				hub = hubPtr(existing<nodePtr>(next));
				next = existing<nodePtr>(hub.hub());
			}
			if (success)
				next.connect(previous);
			else
				hub.connect(previous);
			return success;
		}


		//�������� ��� ���������, ����������� �� ����
		static void getAllPortIterators(std::vector<portIteratorPtr>& iteratorSet,
				existing<nodePtr> node)
		{
			portIterator iter(node);
			do
			{//���� � ���� ����� ��������� �������� ����������� �� ��� ����
				if ((portIteratorPtr::match(iter.get())) &&
					(iter.get().connection(0) == node))
					iteratorSet.push_back(
						iteratorPtr(pointerPtr(existing<nodePtr>(iter.get()))));

			} while (iter.stepForward());
		}
		static void getAllGroupIterators(std::vector<groupIteratorPtr>& iteratorSet,
			existing<nodePtr> node)
		{
			portIterator iter(node);
			do
			{//���� � ���� ����� ��������� �������� ����������� �� ��� ����
				if ((groupIteratorPtr::match(iter.get())) &&
					(iter.get().connection(0) == node))
					iteratorSet.push_back(
						iteratorPtr(pointerPtr(existing<nodePtr>(iter.get()))));

			} while (iter.stepForward());
		}
	private:
		static bool erase(std::vector<portIteratorPtr>& iterSet,
			existing<portIterator> iter)
		{
			if (!hubPtr::match(existing<nodePtr>(iter.getHPPair().hub)))
				return false; // ������� ����� ������ ������������ ����
			hubPtr hub(existing<nodePtr>(iter.getHPPair().hub));
			//������ ������� ���, �� ������ �������������� ��� ��� �����������
			if (!hub.empty())
				return false;
			for (int i = 0; i < iterSet.size(); ++i)
			{
				if (iterSet[i].getHPPair().hub == iter.getHPPair().hub)
				{
					iterSet[i].setHPPair(hubPosPair());//�����
					iterSet[i] = iterSet.back();//�������� �� ������
					iterSet.pop_back();
				}
			}
			creator::deleteHub(hub);
			return true;
		}
		static bool erase(std::vector<groupIteratorPtr>& iterSet,
			existing<groupIterator> iter)
		{
			hubPtr hub(existing<nodePtr>(iter.getHPPair().hub));
			groupPtr group = existing<nodePtr>(iter.getPurpose());
			//������ ������� ���, �� ������ �������������� ��� ��� �����������
			if (!hub.empty())
				return false;
			if (hub == group.firstGroupHub())
				return false;//������ ������� ������ ��� � ������
			for (int i = 0; i < iterSet.size(); ++i)
			{
				if (iterSet[i].getHPPair().hub == iter.getHPPair().hub)
				{
					iterSet[i].setHPPair(hubPosPair());//�����
					iterSet[i] = iterSet.back();//�������� �� ������
					iterSet.pop_back();
				}
			}
			creator::deleteHub(hub);//�������� �������� ����
			return true;
		}
	};
	
}
