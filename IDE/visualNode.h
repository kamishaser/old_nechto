#pragma once
#include "GLM/glm.hpp"
#include "object.h"
#include "nodeOperationSet.h"
#include "creator.h"
#include "visual.h"
#include "rect.h"
#include <codecvt>
#include <locale>
#include <iostream>


namespace nechto::ide
{
	//� ����� 0 ������������ 
	//	������������ ����, ���� visualNode ������������ ��� �����������,
	//	����������� ��������, ���� visualNode ������������ ��� ������
	//� ����� 1 ������������ visualGroup ����������� ���� �����
	//� ����� 3 ������������ ������ ���� vNode � nodeBoard

	struct visualNode:public namedExCon
	{
		//������� ������� �� �������� � ��������� ��������.
		//��� ����� ��� ���������� � ������� �����������
		rect frame;//������� �����

		color shapeColor = col::vNodeG1;
		color lightColor = color(0, 0, 0);
		geometricShape nShape;
		std::wstring nodeText;
	
		visualNode(nodePtr emptyExternalObject, nodePtr v1  = nullptr)
			:namedExCon(emptyExternalObject, L"visualNode"), frame(glm::vec2{0.f, 0.f}, glm::vec2{1.f, 1.f})
			//��� �������� ����, �������� � ��� ������ !!!������ �������� ����� new!!!
		{
			assert(!objectPtr<visualNode>::match(v1));//vNode ������ ���������� � vNode (��������)
			if(v1.exist())
				NumHubConnect(exConNode, v1, 0);
		}
		nodePtr getNodeBoard()
		{
			nodePtr temp = node().connection(3);
			if (!temp.exist())
				return nullptr;
			temp = temp.connection(0);
			return temp;
		}
		const static std::wstring typeName;
		const static staticNodeOperationSet methodSet;
		virtual const std::wstring& getTypeName() const override
		{
			return typeName;
		}
		virtual const operation& getMethod(char number)const override
		{
			return methodSet.getOperation(number);
		}
		
		virtual void serialize(std::vector<char>& buffer, existing<nodePtr> obj) const
		{
			frect temp{ frame.position.x, frame.position.y, frame.size.x, frame.size.y };
			buffer.resize(sizeof(frect));
			char* t = reinterpret_cast<char*>(&temp);
			for (int i = 0; i < sizeof(frect); ++i)
				buffer[i] = t[i];
		}
		struct frect
		{
			float posx;
			float posy;
			float sizex;
			float sizey;
		};
	};
	const std::wstring visualNode::typeName = L"nechtoIde.visualNode";
	const staticNodeOperationSet visualNode::methodSet{};
}
