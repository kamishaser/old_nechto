#pragma once

#include "display.h"
#include "autoExpandHandler.h"
#include "userH.h"
#include "vnDataUpdateH.h"
#include "fileHandler.h"
#include "editor.h"
#include "settingList.h"
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
		settingList sl;
		

		

		nechtoIDE(const std::filesystem::path& path, const std::filesystem::path& directory)
			:gui(directory), vduh(gui), ed(gui, uh.selectH),
			uh(gui, ed, sl),
			aeh(gui),
			fh(gui, directory),
			sl(gui)
		{
			fh.load(path);
		}
		~nechtoIDE()
		{
			fh.save(L"autosave.nechto");
		}
		bool update()
		{
			uh.update();
			ed.update();
			aeh.update();
			vduh.update();
			fh.update();
			
			return gui.update();

		}
	private:
		
	};

}

