#pragma once
#include "ideStructurePack.h"
#include "visual.h"
#include "windowEntity.h"
#include "textOut.h"
#include "factory.h"
#include "event.h"

namespace nechto::ide
{
	class mouseEntity: public singleConnectedEntity
	{
		//буфферы
		//bool atFocus;
		//позиция относительно
		glm::vec2 wMousePos;//окна
		glm::vec2 eMousePos;//элемента
	public:
		bool atInterfaceBoard = false;
		mouseEntity() 
		{
			eConnect(creator::createEntity(entityT::singleConnection));
			fabricate(sPack::mouse::getPlan(), node());
		}
		bool updatePosition(nodePtr ideNode, bool freeMode)
		{
			i64 pressMoveMode;
			sPack::mouse::pressMoveMode / node() >> pressMoveMode;
			if (!updatemWindow(ideNode))
			{
				if (pressMoveMode)
				{
					sPack::mouse::pressMoveMode / node() << 0ll;
					event(sPack::mouse::eventType_PressMoveMode);
				}
				return false;
			}
			sPack::mouse::winPos / node() << wMousePos;
			if (!pressMoveMode)
			{
				freeUpdateVBlock();
				sPack::mouse::velPos / node() << eMousePos;
			}
			else if (!isButtonPressed(sPack::mouse::leftButton))
			{
				sPack::mouse::pressMoveMode / node() << 0ll;
				event(sPack::mouse::eventType_PressMoveMode);
			}
			return true;
		}
		buttonStatus updateLeftButton() const
		{
			return updateButton(sPack::mouse::leftButton / node(),
				sf::Mouse::isButtonPressed(sf::Mouse::Left));
		}
		buttonStatus updateRightButton() const
		{
			return updateButton(sPack::mouse::rightButton / node(),
				sf::Mouse::isButtonPressed(sf::Mouse::Right));
		}
		buttonStatus updateMiddleButton() const
		{
			return updateButton(sPack::mouse::middleButton / node(),
				sf::Mouse::isButtonPressed(sf::Mouse::Middle));
		}
		buttonStatus updateXButton1Button() const
		{
			return updateButton(sPack::mouse::x1Button / node(),
				sf::Mouse::isButtonPressed(sf::Mouse::XButton1));
		}
		buttonStatus updateXButton2Button() const
		{
			return updateButton(sPack::mouse::x2Button / node(),
				sf::Mouse::isButtonPressed(sf::Mouse::XButton2));
		}
		bool isButtonPressed(path p)
		{
			i64 temp;
			if (!(p / node() >> temp))
				return false;
			return (temp % 2 == 1);
		}

	private:
		windowEntity* fwindow = nullptr;
		nodePtr evSource() const { return sPack::mouse::eventSource / node(); };
		nodePtr evStorage() const { return nullptr; }//!!!!!!!!!Надо вставить
		void event(path type, nodePtr content = nullptr) const
		{
			addEvent(evStorage(), evSource(), type / node(), content);
		}
		buttonStatus updateButton(nodePtr bnode, bool nStatus) const
		{
			i64 clickStatusNumber = 0;
			bnode >> clickStatusNumber;
			bool clickStatus = clickStatusNumber & 1ll;
			if (nStatus == clickStatus)
				if (nStatus)
					return buttonStatus::Press;
				else
					return buttonStatus::None;
			bnode << ++clickStatusNumber;
			if (nStatus)
			{
				sPack::mouse::clickStartPoint / node() << wMousePos;
				return buttonStatus::BeginClick;
			}
			else 
			{
				event(sPack::mouse::eventType_ButtonClicked);
				return buttonStatus::EndClick;
			}
		}
		bool updatemWindow(nodePtr ideNode)
		{
			windowEntity* temp = fwindow;
			getmWindow(ideNode);//проверка окна
			if (fwindow == nullptr)
			{
				disconnect(sPack::mouse::current / node(), 0);
				disconnect(sPack::mouse::current / node(), 1);
				if(temp != nullptr)
					event(sPack::mouse::eventType_WindowChanged);
				mOverEl = None;
				return false;
			}
			if(temp != fwindow)
				event(sPack::mouse::eventType_WindowChanged);
			NumHubConnect(sPack::mouse::current / node(), fwindow->node(), 0);
			return true;
		}
		void getmWindow(nodePtr ideNode)
		{
			if (fwindow != nullptr)
			{
				if (mWindow(fwindow))
					return;
			}
			fwindow = nullptr;
			nodePtr wgNode = sPack::nechtoIde::windowGroup / ideNode;
			if (!groupPtr::match(wgNode))
				return;
			groupPointer gi(wgNode);
			do
			{
				if (teptr<windowEntity>::match(gi.get()))
				{
					auto temp = teptr<windowEntity>(gi.get()).get();
					if (mWindow(temp))
					{
						fwindow = temp;
						return;
					}
				}
			} while (gi.stepForward());
		}
		bool mWindow(const windowEntity* fWindow)
		{
			wMousePos = GLM_SFML(sf::Mouse::getPosition(fWindow->window));
			return
				wMousePos.x > 0. &&
				wMousePos.x < fWindow->window.getSize().x&&
				wMousePos.y > 0. &&
				wMousePos.y < fWindow->window.getSize().y;
		}
		enum
		{
			None,
			Group,
			Connection,
			Node,
		} mOverEl;
		
		nodePtr freeUpdateVBlock()
		{
			if (isButtonPressed(sPack::mouse::leftButton)/* ||
				isButtonPressed(sPack::mouse::rightButton)*/)
			{
				glm::vec2 clickStartPos{ 0., 0. };
				glm::vec2 currentPos{ 0., 0. };
				sPack::mouse::clickStartPoint / node() >> clickStartPos;
				sPack::mouse::winPos / node() >> currentPos;
				if (glm::abs(clickStartPos.x - currentPos.x) < 5 ||
					glm::abs(clickStartPos.y - currentPos.y) < 5)
				{
					event(sPack::mouse::eventType_PressMoveMode);
					sPack::mouse::pressMoveMode / node() << 1ll;
				}
			}
			
			nodePtr vEl = findvBlock(wMousePos);
			if (vEl != sPack::mouse::curVel / node())
			{
				//оповещение о изменении наведённого элемента
				event(sPack::mouse::eventType_OveredChanged, vEl);
				if (vEl.exist())
				{
					NumHubConnect(sPack::mouse::current / node(), vEl, 1);
				}
				else
					disconnect(sPack::mouse::current / node(), 1);
			}
			return vEl;
		}
		nodePtr findvBlock(glm::vec2 point)
		{
			atInterfaceBoard = true;
			fwindow->interfaceBoardView();
			glm::vec2 pointAtBoard = fwindow->getPointAtBoard(point);
			nodePtr activeVEl = findVELAtNodeBoard(
				sPack::window::interfaceBoard / fwindow->node(), pointAtBoard);
			if (activeVEl.exist())
				return activeVEl;
			atInterfaceBoard = false;
			fwindow->workBoardView();
			pointAtBoard = fwindow->getPointAtBoard(point);
			return findVELAtNodeBoard(
				sPack::window::workBoard / fwindow->node(), pointAtBoard);
		}
		nodePtr findVELAtNodeBoard(nodePtr nBoard, glm::vec2 point)
		{
			nodePtr node = 
				findBlockAtGroup(sPack::nBoard::vNodeGroup / nBoard, point);
			mOverEl = Node;
			if (node.exist())
				return node;
			node =
				findConnectionAtGroup(sPack::nBoard::vConnectionGroup / nBoard, point);
			mOverEl = Connection;
			if (node.exist())
				return node;
			node = 
				findBlockAtGroup(sPack::nBoard::vGroupGroup / nBoard, point);
			mOverEl = Group;
				if (node.exist())
					return node;
			mOverEl = None;
			return nullptr;
		}
		nodePtr findBlockAtGroup(nodePtr group, glm::vec2 point)
		{
			if (!groupPtr::match(group))
				return nullptr;
			groupPointer gi(group);
			do
			{
				if (vBlockUnder("0"_np / gi.get(), point))
					return "0"_np / gi.get();
			} while (gi.stepForward());
			return nullptr;
		}
		nodePtr findConnectionAtGroup(nodePtr group, glm::vec2 point)
		{
			if (!groupPtr::match(group))
				return nullptr;
			groupPointer gi(group);
			do
			{
				if (vConnectionUnder("0"_np / gi.get(), point))
					return "0"_np / gi.get();
			} while (gi.stepForward());
			return nullptr;
		}
		bool vBlockUnder(nodePtr vBlock, glm::vec2 point)
		{
			if (!vBlock.exist() || !sPack::rect::match(sPack::vNode::frame / vBlock))
				return false;
			rect frame(sPack::vNode::frame / vBlock);
			if (!frame.contains(point))
				return false;
			eMousePos = point - frame.position;
			return true;
		}
		bool vConnectionUnder(nodePtr vConnection, glm::vec2 point)
		{
			return false;
		}
	};
}
