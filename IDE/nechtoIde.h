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
		
	};

}

