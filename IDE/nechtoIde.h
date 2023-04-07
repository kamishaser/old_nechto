#pragma once

#include <iostream>
#include "drawer.h"
#include "windowEntity.h"
#include "ideFactory.h"
#include "textOut.h"
#include "consistentGroup.h"
#include "illuminationHandler.h"
#include "interactiveHanler.h"
#include "selectHandler.h"
#include "textInputField.h"
#include "userH.h"

#include "button.h"

namespace nechto::ide
{
	//при удалении ноды надо удалить все итераторы
	//при группы надо удалить все итераторы
	//хабы должны удаляться не в deleteNode
	//оптимизация eraseHub в итераторе или нормальное удаление всех итераторов
	class nechtoIDE : public singleConnectedEntity
	{
		ideFactory fact;
		windowEntity we;
		drawer draw;
		userH uh;
		illuminationHandler ilh;
		selectHandler sh;
		interactiveHandler inh;
		textInputField tef;
	public:
		nodePtr consisGr;
		nodePtr button;
		nechtoIDE()
		{
			eConnect(creator::createEntity(entityT::singleConnection));
			fact.clearAndFabricate(sPack::nechtoIde::getPlan(), node());
			fact.clearAndFabricate(sPack::window::getPlan(), we.node());
			we.connectToIde(node());
			fact.clear();
			connectAllServices();
			testStart();
		}
		~nechtoIDE()
		{
		}
		bool update()
		{
			
			testUpdate();
			uh.update(node());
			ilh.update();
			inh.update();
			sh.update();
			tef.update();
			draw.update(&we.window, we.node());
			return we.update();
		}
	private:
		void connectAllServices()
		{
			NumNumConnect("00"_np / node(), uh.mouse.node(), 0, 3);
			PointerNumConnect(backGroupPort(sPack::nechtoIde::windowGroup / node()),
				we.node(), 3);
			ilh.finalConnect(node());
			sh.finalConnect(node());
			inh.finalConnect(node());
			tef.finalConnect(node());
		}
		void testStart()
		{
			rect r(200, 200, 0, 0);
			consisGr = fact.fabricateVGroup(sPack::window::workBoard / we.node(),
				nullptr, sPack::vGroup::gType::consistent);
			sPack::vGroup::drawableBlock / sPack::draBlock::frame / consisGr << r;
			consistentGroup cgr(consisGr, fact);
			//cgr.mode.reverseDirection = true;
			cgr.mode.horisontal = true;
			//cgr.mode.rightAlignment = true;
			sPack::vGroup::data / consisGr << cgr.mode;
			r.size = glm::vec2(100.f, 100.f);
			nodePtr oldNode = nullptr;
			for (int i = 0; i < 6; ++i)
			{
				fact.clear();
				nodePtr vNode = fact.fabricateVNode(sPack::window::workBoard / we.node(), consisGr);
				fact.clear();
				if (oldNode != nullptr)
					fact.fabricateVConnection(oldNode, vNode, sPack::window::workBoard / we.node(), consisGr);
				oldNode = vNode;
				sPack::vNode::drawableBlock / sPack::draBlock::frame / vNode << r;
			}
			r.position = glm::vec2(10., 10);
			fact.clear();
			nodePtr vNode = fact.fabricateVNode(sPack::window::interfaceBoard / we.node());
			sPack::vNode::drawableBlock / sPack::draBlock::text / vNode << L"testButton";
			sPack::vNode::drawableBlock / sPack::draBlock::frame / vNode << r;

			button = button::fabricate(inh.node(), vNode);
		}
		void testUpdate()
		{
			consistentGroup(consisGr, fact).update();
			if (button::isClicked(button))
				std::wcout << L"click" << std::endl;
		}
	};

}

