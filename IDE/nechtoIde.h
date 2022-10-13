#pragma once

#include "display.h"
#include "autoExpandHandler.h"
#include "userH.h"
#include "vnDataUpdateH.h"
#include "fileSerializer.h"

namespace nechto::ide
{
	//��� �������� ���� ���� ������� ��� ���������
	//��� ������ ���� ������� ��� ���������
	//���� ������ ��������� �� � deleteNode
	//����������� eraseHub � ��������� ��� ���������� �������� ���� ����������
	class nechtoIDE
	{
	public:
		display dp;
		vnDataUpdateH vduh;
		autoExpandHandler aeh;
		userH uh;

		

		nechtoIDE()
			:dp(),
			vduh(dp),
			uh(dp),
			aeh(dp)			
		{
			IterIterConnect(group::firstEmptyPort(dp.workBoard.taggedGroup()),
				connectionIterator(uh.mouse.cursor.get(), 1));
			IterIterConnect(group::firstEmptyPort(dp.interfaceBoard.taggedGroup()),
				connectionIterator(uh.mouse.cursor.get(), 1));
			dp.load(L"autosave.nechto");
		}
		~nechtoIDE()
		{
			dp.save(L"autosave.nechto");
		}
		bool update()
		{
			uh.update();
			aeh.update();
			vduh.update();
			
			return dp.update();

		}
	private:
		nodePtr example()
		{
			nodePtr descriptionText = newNode(node::Text);
			text::set(descriptionText, std::wstring(L"���������� ��������\n") +
				L"��� ���������� � ������������" +
				L"������� �������� ����������� nechto");
			
			nodePtr vFirst= nullNodePtr;
			nodePtr vLast = nullNodePtr;
			for (int i = 0; i < 16; ++i)
			{
				nodePtr v1 = newNode(node::Variable, true);
				v1->setData<i64>(i);
				if (vLast.exist())
					HubHubConnect(v1, vLast);
				else
					vFirst = v1;
				vLast = v1;
			}
			return vFirst;
		}
		
	};

}

