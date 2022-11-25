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
		bool eraseHub(existing<portIterator> iter)
		{
			if (!hubPtr::match(existing<nodePtr>(iter.getHPPair().hub)))
				return false; // ������� ����� ������ ������������ ����
			hubPtr hub(existing<nodePtr>(iter.getHPPair().hub));
			//������ ������� ���, �� ������ �������������� ��� ��� �����������
			if (!hub.empty())
				return false;

			nodePtr previous = hub.previous();//���������� ���
			std::vector<iteratorPtr> iterSet;//������ ���� ���������� �� ��� ����

			getAllPortIterators(iterSet, iter.getPurpose());
			for (auto& i : iterSet)
				resetIterator(i, iter);//����� ���� ���������� �� ��� ���
			nodePtr next = hub.hub();//��������� ���
			//��������� ������� � ����������� ���������� ����
			previous.node()->hubPort = next;
			if (next.exist())
				hubPtr(next).setPrevious(previous);
			creator::deleteHub(hub);
			return true;
		}
		//�������� ���� �� ������
		bool eraseHub(existing<groupIterator> iter)
		{
			hubPtr hub(existing<nodePtr>(iter.getHPPair().hub));
			groupPtr group = existing<nodePtr>(iter.getPurpose());
			//������ ������� ���, �� ������ �������������� ��� ��� �����������
			if (!hub.empty())
				return false;

			nodePtr previous = hub.previous();//���������� ���
			if (previous == hub)//������ ������� ��������� ��� � ������
				return false;
			std::vector<iteratorPtr> iterSet;//������ ���� ���������� �� ��� ����

			getAllPortIterators(iterSet, iter.getPurpose());
			for (auto& i : iterSet)
				resetIterator(i, iter);//����� ���� ���������� �� ��� ���
			hubPtr next = existing<nodePtr>(hub.hub());//��������� ���
			if (hub == group.firstGroupHub())//���� ��������� ��� �������� ������
				group.setFirstGroupHub(next);

			//��������� ������� � ����������� ���������� ����
			if (next.exist())
				hubPtr(next).connect(previous);
			else
				previous.node()->hubPort = nullptr;
			creator::deleteHub(hub);//�������� �������� ����
			return true;
		}
		
		//�������� ���� �����
		//bool eraseHubChain(portIterator begin, portIterator end)
		//{
		//	if (!hubPtr::match(existing<nodePtr>(begin.getHPPair().hub)))
		//		return false; // ������� ����� ������ ������������ ����

		//	//���� begin � end ��������� �� ������ ����, �������� ������
		//	if (begin.getPurpose() != end.getPurpose())
		//		return false;
		//	
		//	nodePtr previous = hubPtr(existing<nodePtr>(begin.getHPPair().hub)).previous();//���������� ���
		//	std::vector<iteratorPtr> iterSet;//������ ���� ���������� �� ��� ����
		//	getAllPortIterators(iterSet, begin.getPurpose());

		//	while (true)
		//	{
		//		hubPtr hub(existing<nodePtr>(begin.getHPPair().hub));
		//		if (!hub.empty())
		//		{//���� � ���� ���� ����������� ��� �� ��� ��������� �������� end
		//			//�������� previous � 
		//			previous.node()->hubPort = hub.hub();
		//			if (hub.hub())
		//				hubPtr(hub.hub()).setPrevious(previous);
		//			return true;
		//		}
		//		if (hub == end.getHPPair().hub)
		//		{//���� � ���� ���� ����������� ��� �� ��� ��������� �������� end
		//		}
		//	}
		//}
		//bool eraseHubChain(groupIterator begin, groupIterator end);


		//�������� ��� ���������, ����������� �� ����
		static std::vector<iteratorPtr>
			getAllPortIterators(std::vector<iteratorPtr>& iteratorSet,
				existing<nodePtr> node)
		{
			portIterator iter(node);
			do
			{
				if (iteratorPtr::match(iter.get()))
					if (iter.get().connection(0) == node)
						iteratorSet.push_back(
							pointerPtr(existing<nodePtr>(iter.get())));

			} while (iter.stepForward());
			return iteratorSet;
		}
		static void resetIterator(iteratorPtr& nodeIter, iterator iter)
		{
			if (nodeIter.getHPPair().hub == iter.getHPPair().hub)
				nodeIter.setHPPair(hubPosPair());
		}
	private:
		static void erase(std::vector<iteratorPtr>& iteratorSet,
			existing<portIterator> iter)
		{
			
		}

	};
	
}
