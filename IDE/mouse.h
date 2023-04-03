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
			factory fact;
			fact.fabricate(sPack::mouse::getPlan(), node());
		}
		bool updatePosition(nodePtr ideNode, windowEntity* fwindow, bool freeMode)
		{
			if (!updatemWindow(ideNode, fwindow))
				return false;
			sPack::mouse::winPos / node() << wMousePos;
			if (freeMode)
			{
				freeUpdateVBlock(fwindow);
				sPack::mouse::velPos / node() << eMousePos;
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

		//источник событий и тип событий
		nodePtr evSource = fabricate(sPack::eventSource::getPlan());
		nodePtr evType = fabricate(sPack::eventType::getPlan());
		groupPtr evStorage = creator::createGroup(groupT::strong);

	private:
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
				return buttonStatus::EndClick;
		}
		bool updatemWindow(nodePtr ideNode, windowEntity*& fwindow)
		{
			getmWindow(ideNode, fwindow);//проверка окна
			if (fwindow == nullptr)
			{
				disconnect(sPack::mouse::current / node(), 0);
				disconnect(sPack::mouse::current / node(), 1);
				mOverEl = None;
				return false;
			}
			NumHubConnect(sPack::mouse::current / node(), fwindow->node(), 0);
			return true;
		}
		void getmWindow(nodePtr ideNode, windowEntity*& fWindow)
		{
			if (fWindow != nullptr)
			{
				if (mWindow(fWindow))
					return;
			}
			fWindow = nullptr;
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
						fWindow = temp;
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
		
		nodePtr freeUpdateVBlock(windowEntity* win)
		{

			nodePtr vEl = findvBlock(win, wMousePos);
			if (vEl != sPack::mouse::curVel / node())
			{
				//nodePtr drablock = sPack::vNode::drawableBlock
				//sPack::vNode::drawableBlock / sPack::draBlock::illuminationThinkness
				//какое-то действие с элементом // запуск события
				if (vEl.exist())
				{
					NumHubConnect(sPack::mouse::current / node(), vEl, 1);
					std::wcout << to_string(vEl) << std::endl;
					addEvent(evStorage, evSource, evType);
				}
				else
					disconnect(sPack::mouse::current / node(), 1);
			}
			return vEl;
		}
		nodePtr findvBlock(windowEntity* win, glm::vec2 point)
		{
			atInterfaceBoard = true;
			win->interfaceBoardView();
			glm::vec2 pointAtBoard = win->getPointAtBoard(point);
			nodePtr activeVEl = findVELAtNodeBoard(
				sPack::window::interfaceBoard / win->node(), pointAtBoard);
			if (activeVEl.exist())
				return activeVEl;
			atInterfaceBoard = false;
			win->workBoardView();
			pointAtBoard = win->getPointAtBoard(point);
			return findVELAtNodeBoard(
				sPack::window::workBoard / win->node(), pointAtBoard);
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
