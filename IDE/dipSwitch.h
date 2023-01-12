#pragma once
#include "buttonList.h"
#include "GUI.h"

namespace nechto::ide
{
	class dipSwitch
	{
		GUI& gui;
		sharedButton button;
		bool status = false;
		
	public:
		dipSwitch(GUI& g, const std::wstring& name)
			:gui(g), button(name)
		{}
		sharedButton* getButton()
		{
			return &button;
		}
		const sharedButton& operator*() const
		{
			return button;
		}
		operator sharedButton* ()
		{
			return &button;
		}
		const std::wstring& name() const
		{
			return button.name;
		}
		const bool test() const
		{
			return status;
		}
		void set()
		{
			gui.activeButton(&button);
			status = true;
		}
		void reset()
		{
			gui.resetButton(&button);
			status = false;
		}
		bool change()
		{
			if (status)
				reset();
			else
				set();
			return status;
		}
		bool updateStatus()//обновление статуса кнопки
		{//позволяет синзранизировать status и связь при некоректном переключении
			if (gui.isButtonActive(&button))
				status = true;
			else
				status = false;
		}
	};
}