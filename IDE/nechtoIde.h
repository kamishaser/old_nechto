#pragma once

#include "display.h"
#include "autoExpandHandler.h"
#include "userH.h"
#include "vnDataUpdateH.h"
#include "fileHandler.h"
#include "editor.h"
#include <iostream>

namespace nechto::ide
{
	//��� �������� ���� ���� ������� ��� ���������
	//��� ������ ���� ������� ��� ���������
	//���� ������ ��������� �� � deleteNode
	//����������� eraseHub � ��������� ��� ���������� �������� ���� ����������
	class nechtoIDE
	{
	public:
		GUI gui;
		editor ed;
		vnDataUpdateH vduh;
		autoExpandHandler aeh;
		fileHandler fh;
		userH uh;
		

		

		nechtoIDE()
			:vduh(gui), ed(gui, uh.selectH),
			uh(gui, ed),
			aeh(gui),
			fh(gui)
		{
			fh.load(L"autosave.nechto");
		}
		~nechtoIDE()
		{
			fh.save(L"autosave.nechto", gui.workBoard.vNodeGroup());
		}
		bool update()
		{
			uh.update();
			ed.update();
			aeh.update();
			vduh.update();
			
			return gui.update();

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

