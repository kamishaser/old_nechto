#pragma once
#include "ideStructurePack.h"
#include "windowEntity.h"
#include "mouse.h"

namespace nechto::ide
{
	class userH : public singleConnectedEntity
	{
		windowEntity* activeWindow = nullptr;//активное в данных момент окно
		//activeHandler//обработчик активных элементов
		//последовательна€ группа активных элементов
		bool textInputActive = false; //активен ввод текста
		//keyboardHandler //выбирает активное сочетание клавиш
		//selectHandler//выделенные элементы workBoard
		//editor//редактор элементов
		//illuminationHandler//обработчик подсветки
		//logger//сохранение действий дл€ возможной последующей их отмены
		
		enum class actionMode
		{
			free,
			scroll,
			shifting,
			textSelection,
			frameSelection,
			textInput,
			interacriveTool
		};
		actionMode mode = actionMode::free;
	public: 
		mouseEntity mouse;
		userH()
		{
			eConnect(creator::createEntity(entityT::singleConnection));
		}
		bool update(nodePtr ideNode)
		{
			if (activeWindow == nullptr || !activeWindow->window.hasFocus())
				if (!findActiveWindow(ideNode))
					return false;//обновить не удалось, так как нет активного окна
			mouse.updatePosition(ideNode, mode == actionMode::free);
			switch (mode)
			{
			case nechto::ide::userH::actionMode::free:
				updateFree();
				break;
			case nechto::ide::userH::actionMode::scroll:
				updateScroll();
				break;
			case nechto::ide::userH::actionMode::shifting:
				updateShifting();
				break;
			case nechto::ide::userH::actionMode::textSelection:
				updateTextSelection();
				break;
			case nechto::ide::userH::actionMode::frameSelection:
				updateFrameSelection();
				break;
			case nechto::ide::userH::actionMode::textInput:
				updateTextInput();
				break;
			case nechto::ide::userH::actionMode::interacriveTool:
				updateInteractiveTool();
				break;
			default:
				assert(false);
			}
			return true;
		}
		bool reset()//сброс активного режима в свободный с завершением действий
		{
			switch (mode)
			{
			case nechto::ide::userH::actionMode::scroll:
				endScroll();
				break;
			case nechto::ide::userH::actionMode::shifting:
				endShifting();
				break;
			case nechto::ide::userH::actionMode::textSelection:
				endTextSelection();
				break;
			case nechto::ide::userH::actionMode::frameSelection:
				endFrameSelection();
				break;
			case nechto::ide::userH::actionMode::textInput:
				endTextInput();
				break;
			case nechto::ide::userH::actionMode::interacriveTool:
				endInteractiveTool();
				break;
			default:
				return false;
			}
			mode = actionMode::free;
			return true;
		}
		void activeTextInput()
		{
			if (mode != actionMode::free)
				reset();
			mode = actionMode::textInput;
			assert(false);//недоделано
		}
		
	private:
		void updateFree()
		{
			
			switch (mouse.updateLeftButton())
			{
			case buttonStatus::EndClick:
				leftClick();
				break;
			case buttonStatus::Press:
				if (sStart())
				{
					if (+(sPack::mouse::curVel / mouse.node()))
						if (!mouse.atInterfaceBoard)
							mode = actionMode::shifting;
						else;//тут возможно выделение текста и 
					//взаимодействие с интерактивными инструментами
					else
						mode = actionMode::frameSelection;
					return;
				}
				break;
			default:
				break;
			}
			if (mouse.updateRightButton() == buttonStatus::EndClick)
				righitClick();
			switch (mouse.updateMiddleButton())
			{
			case buttonStatus::EndClick:
				middleClick();
				break;
			case buttonStatus::Press:
				if (sStart())
				{
					mode = actionMode::scroll;
					return;
				}
				break;
			default:
				break;
			}
			//тут надобно обновить сочетани€ клавиш
		}
		void updateScroll()
		{
			if (sResetEvent())
				endScroll();
		}
		void updateShifting()
		{
			if (sResetEvent())
				endScroll();
		}
		void updateFrameSelection()
		{
			if (sResetEvent())
				endFrameSelection();
		}
		void updateTextSelection()
		{
			if (sResetEvent())
				endTextSelection();
		}
		void updateTextInput()
		{
			if (sResetEvent())
				endTextInput();
		}
		void updateInteractiveTool()
		{
			if (sResetEvent())
				endInteractiveTool();
		}
		void endTextInput()
		{
			mode = actionMode::free;
		}
		void endScroll()
		{
			mode = actionMode::free;
		}
		void endShifting()
		{
			mode = actionMode::free;
		}
		void endTextSelection()
		{
			mode = actionMode::free;
		}
		void endFrameSelection()
		{
			mode = actionMode::free;
		}
		void endInteractiveTool()
		{
			mode = actionMode::free;
		}
	
		void leftClick()
		{
			if (+(sPack::mouse::curVel / mouse.node()))
			{
				if (mouse.atInterfaceBoard)
				{
					//обновл€ю кнопку
				}
				else
				{
					//обрабатываю выделение
				}
			}
			else
			{
				//ничего
			}
		}
		void righitClick()
		{
			if (+(sPack::mouse::curVel / mouse.node()))
			{
				if (mouse.atInterfaceBoard)
				{
					//список возможных действий +
					//запрашиваю информацию по активному элементу (ещЄ не определилс€)
				}
				else
				{
					//список возможный действий + включение режима редактировани€
				}
			}
			else
			{
				//взаимодействие с пустотой, способ перехода или 
				//создание объекта вне настроек редактора
			}

		}
		void middleClick()
		{
			if (!+ (sPack::mouse::curVel / mouse.node()))
			{
				//создаю новую ноду или группу по настройкам редактора
			}
		}
		bool findActiveWindow(nodePtr ideNode)
		{
			nodePtr grNode = sPack::nechtoIde::windowGroup / ideNode;
			if (!groupPtr::match(grNode))
				return false;
			groupPointer gi(grNode);
			do
			{
				if(teptr<windowEntity>::match(gi.get()))
					if (teptr<windowEntity>(gi.get()).get()->window.hasFocus())
					{
						activeWindow = teptr<windowEntity>(gi.get()).get();
						return true;
					}
			} while (gi.stepForward());
			return false;
		}
		bool sResetEvent() const//сброс scroll и selection
		{
			buttonStatus lb = mouse.updateLeftButton();
			buttonStatus rb = mouse.updateRightButton();
			buttonStatus mb = mouse.updateMiddleButton();
			buttonStatus x1b = mouse.updateXButton1Button();
			buttonStatus x2b = mouse.updateXButton2Button();
			if (lb > buttonStatus::Press ||
				rb > buttonStatus::Press ||
				mb > buttonStatus::Press)
				return true;
			return false;
		}
		const float sLenght = 6.f;
		bool sStart()
		{
			glm::vec2 curPos(0.f, 0.f);
			if (!(sPack::mouse::winPos / mouse.node() >> curPos))
				return false;
			glm::vec2 clickStartPos(0.f, 0.f);
			if (!(sPack::mouse::clickStartPoint / mouse.node() >> clickStartPos))
				return false;
			if (glm::length(curPos - clickStartPos) > sLenght)
				return true;
			return false;
		}
	};
}
//
//
//namespace nechto::ide
//{
//	//обработчик пользовательских действий
//	class userH
//	{
//	public:
//		GUI& gui;
//		editor& ed;
//		settingList& settings;
//		keyboardHandler keyboard;
//		selectHandler selectH;
//		mouseHandler mouse;
//
//		sharedButton testButton;
//		
//		userH(GUI& g, editor& e, settingList& sl)
//		:gui(g), ed(e), selectH(), mouse(g, keyboard, selectH), settings(sl),
//		testButton(L"testButton")
//		{
//			gui.addButton(&testButton, &gui.bottomGroup);
//		}
//
//		void updateTestButton()
//		{
//			if (testButton.bClickEvent())
//			{
//				if (gui.isButtonActive(&testButton))
//					gui.resetButton(&testButton);
//				else
//					gui.activeButton(&testButton);
//			}
//		}
//
//		void update()
//		{
//			mouse.update();
//			if (mouse.rightButton.bClickEvent())
//			{
//				auto vNode1 =
//					getEntity<visualNode>(mouse.rightButton.content());
//				if (vNode1)
//					editText(vNode1);
//				else
//					gui.textBox.reset();
//			}
//			if (mouse.middleButton.bClickEvent())
//				addNode();
//			if (!gui.textBox.hasFocus())
//			{
//				keyboard.update();
//				if (keyboard[sf::Keyboard::N].bClickEvent())//создание
//					addNode();
//				if (keyboard[sf::Keyboard::Delete].bClickEvent())//удаление
//					deleteAllSelected();
//				if (keyboard[sf::Keyboard::C].bClickEvent())//подключение
//					ed.eConnect(getEntity<visualNode>(selectH.lastSelected()));
//				if (keyboard[sf::Keyboard::D].bClickEvent())//отключение
//					disconnectSelected();
//				if (keyboard[sf::Keyboard::A].bClickEvent())//выделение всех
//					selectH.selectGroup(gui.workBoard.vNodeGroup());
//				if (keyboard[sf::Keyboard::R].bClickEvent())//рандомное смещение всех
//					randomOfsetSelected();
//				if (keyboard[sf::Keyboard::T].bClickEvent())//ввод текста дл€ команды
//					gui.textBox.focus(L"введите текст", nullptr);
//				//if (keyboard[sf::Keyboard::S].bClickEvent())//выполнение помледнего выделенного действи€
//				//	operateLSelected();
//				if (keyboard[sf::Keyboard::Tab].isPressed())//описание соединений
//					if (mouse.cursored())
//						gui.cursoredParametrs.nodeText =
//						connectionsList(mouse.cursored()->node().connection(0));
//					else;
//				else if (settings.testButton.test())
//					gui.cursoredParametrs.nodeText = getStringNumberOfNodesOfAllTypes();
//				else
//					gui.cursoredParametrs.nodeText =
//					std::to_wstring(nodeStorage::terminal.numberOfNodes) + L"\n" +
//					std::to_wstring(entity::numberOfEntitys);
//			}
//			else
//			{
//				for (auto& i : keyboard.keyBoardKeys)
//					i.bClickEvent();
//				mouse.rightButton.bClickEvent();
//				mouse.middleButton.bClickEvent();
//			}
//			updateTestButton();
//		}
//		void addNode()
//		{
//			auto vNode = ed.addNode();
//			vNode->frame.setPositionByCenter(mouse.cursorPosition());
//			editText(vNode);
//		}
//		void editText(visualNode* vNode1)
//		{
//			nodePtr v1 = vNode1->node().connection(0);
//			if (v1.exist())
//			{
//				gui.textBox.focus(L"введите данные ноды", vNode1);
//			}
//		}
//		void deleteAllSelected()
//		{
//			groupPointer gi(selectH.selectedGroup());
//			do
//			{
//				if (gi.get().exist())
//				{
//					if (gi.get().connection(0).exist())
//						creator::deleteNode(gi.get().connection(0));
//					creator::deleteNode(gi.get());
//				}
//			} while (gi.stepForward());
//		}
//
//		void connectSelected(visualNode* vNode1)
//		{
//			if (vNode1 == nullptr)
//				return;
//			groupPointer gi(selectH.selectedGroup());
//			do
//			{
//				auto vNode2 = getEntity<visualNode>(gi.get());
//				if (vNode2)
//				{
//					nodePtr v1 = vNode1->node().connection(0);
//					nodePtr v2 = vNode2->node().connection(0);
//					if (v1 != v2)
//					{
//						if (v1.exist() && v2.exist() && !hasConnection(v1, v2))
//						{
//							HubHubConnect(v1, v2);
//						}
//						if (!gui.workBoard.connected(getEntityPtr(vNode1), 
//							getEntityPtr(vNode2)))
//						{
//							gui.workBoard.addConnection(visualConnection::create(
//								getEntityPtr(vNode1),
//								getEntityPtr(vNode2)));
//						}
//					}
//				}
//			} while (gi.stepForward());
//		}
//		void disconnectSelected()
//		{
//			if (!selectH.lastSelected().exist())
//				return;
//			groupPointer gi(selectH.selectedGroup());
//			do
//			{
//				if (gi.get().exist() && gi.get() != selectH.lastSelected())
//				{
//					nodePtr v1 = gi.get().connection(0);
//					nodePtr v2 = selectH.lastSelected().connection(0);
//					if (v1.exist() && v2.exist())
//						nearestDisconnect(v1, v2);
//				}
//			} while (gi.stepForward());
//		}
//		void randomOfsetSelected()
//		{
//			groupPointer gi(selectH.selectedGroup());
//			do
//			{
//				auto vNode = getEntity<visualNode>(gi.get());
//				if (vNode)
//					vNode->frame.position += randomOffset(100);
//			} while (gi.stepForward());
//		}
//		/*void operateLSelected()
//		{
//			nodePtr lSelected = selectH.lastSelected();
//			if (!lSelected.exist())
//				return;
//			lSelected = lSelected.connection(0);
//			if (!lSelected.exist())
//				return;
//			if (!(isAction(lSelected) && check(lSelected)))
//				return;
//			
//			lSelected = step(lSelected);
//			if (!lSelected.exist())
//				return;
//			portPointer ci(lSelected);
//			do
//			{
//				auto vNode = getEntity<visualNode>(ci.get());
//				if (vNode)
//				{
//					selectH.select(vNode);
//					break;
//				}
//			} while (ci.stepForward());
//		}*/
//	};
//}