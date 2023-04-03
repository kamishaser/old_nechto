#pragma once
#include "structure.h"
#include "factory.h"
#include "ideStructurePack.h"
#include "portSearch.h"

namespace nechto::ide
{
	class ideFactory : public factory
	{
	public:
		nodePtr fabricateVNode(nodePtr nodeBoard, nodePtr vGroup)
		{
			nodePtr intermed = creator::createStruct(0);
			fabricate(sPack::vNode::getPlan(), intermed);
			nodePtr group = sPack::nBoard::vNodeGroup / nodeBoard;
			if(groupPtr::match(group))
				PointerNumConnect(backGroupPort(group), intermed, 3);
			group = sPack::vGroup::vBlockGroup / vGroup;
			if (groupPtr::match(group))
				PointerNumConnect(backGroupPort(group), intermed, 1);
			return finishedParts.at(L"vNode");
		}
		//создание нового соединения. !Внимание!, отсутствует защита
		nodePtr fabricateVConnection(nodePtr vNode0, 
			nodePtr vNode1, nodePtr nodeBoard, nodePtr vGroup)
		{
			nodePtr intermed = creator::createStruct(0);
			fabricate(sPack::vConnection::getPlan(), intermed);
			nodePtr group = sPack::nBoard::vConnectionGroup / nodeBoard;
			if (groupPtr::match(group))
				PointerNumConnect(backGroupPort(group), intermed, 3);
			group = sPack::vGroup::vConnectionGroup / vGroup;
			if (groupPtr::match(group))
				PointerNumConnect(backGroupPort(group), intermed, 1);
			nodePtr vConNode = finishedParts.at(L"vConnection");
			PointerNumConnect(sPack::vNode::newConnectionPort(vNode0), vConNode, 0);
			PointerNumConnect(sPack::vNode::newConnectionPort(vNode1), vConNode, 1);
			return vConNode;
		}
		nodePtr fabricateVGroup(nodePtr nodeBoard, nodePtr vGroup, 
			sPack::vGroup::gType t = sPack::vGroup::gType::simple)
		{
			nodePtr intermed = creator::createStruct(0);
			fabricate(sPack::vGroup::getPlan(t), intermed);
			nodePtr group = sPack::nBoard::vGroupGroup / nodeBoard;
			if (groupPtr::match(group))
				PointerNumConnect(backGroupPort(group), intermed, 3);
			group = sPack::vGroup::vBlockGroup / vGroup;
			if (groupPtr::match(group))
				PointerNumConnect(backGroupPort(group), intermed, 1);
			return finishedParts.at(L"vGroup");
		}
	};
}