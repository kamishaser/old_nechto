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
	//при удалении ноды надо удалить все итераторы
	//при группы надо удалить все итераторы
	//хабы должны удаляться не в deleteNode
	//оптимизация eraseHub в итераторе или нормальное удаление всех итераторов
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
		

		

		nechtoIDE()
			:vduh(gui), ed(gui, uh.selectH),
			uh(gui, ed, sl),
			aeh(gui),
			fh(gui),
			sl(gui)
		{
			fh.load(L"autosave.nechto");
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

