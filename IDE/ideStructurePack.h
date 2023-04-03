#pragma once
#include "structure.h"
#include "factory.h"
#include "pExchange.h"

namespace nechto::ide::sPack
{

	namespace vec2
	{
		const path
			x{ "0"_np },
			y{ "1"_np };
		constexpr serialPlan getPlan()
		{
			return serialPlan
			{
				{L"x", sType::f64v(0.)},
				{L"y", sType::f64v(0.)}
			};
		}
		bool match(nodePtr node)
		{
			return
				variablePtr::match(x / node) &&
				variablePtr::match(y / node);
		}
	}
	namespace rect
	{
		const path
			position{ "0"_np },
			size{ "1"_np },
			posX{ position / vec2::x },
			posY{ position / vec2::y },
			sizeX{ size / vec2::x },
			sizeY{ size / vec2::y };
		constexpr serialPlan getPlan()
		{
			return
			{
				{L"position", vec2::getPlan()},
				{L"size", vec2::getPlan()},
				{nullptr}
			};
		}
		bool match(nodePtr node)
		{
			return
				vec2::match(position / node) &&
				vec2::match(size / node);
		}
	}
	namespace draBlock
	{
		const path
			frame{ "0"_np },
			text{ "1"_np },
			blockColor{ "200"_np },
			textColor{ "201"_np },
			illuminationColor{ "202"_np },
			blockShape{ "21"_np },
			charSize{ "221"_np },
			illuminationThinkness{ "222"_np };
		constexpr serialPlan getPlan()
		{
			return serialPlan
			{
				{L"frame", rect::getPlan()},
				{L"text", sType::text(1)},
				{L"param",
					{
						{L"color",
							{
								{L"block", sType::i64v(5)},
								{L"text", sType::i64v(7)},
								{L"illumination", sType::i64v(0)}
							}},
						{L"formKey", sType::i64v(0)},
						{L"size",
							{
								{L"text", sType::f64v(0)},
								{L"illumination", sType::f64v(0)}
							}}
					}}
			};
		};
		bool match(nodePtr node)//���������� �������� ��� �����������
		{//��� �� �������� - �� ���������
			return
				rect::match(frame / node);
		}
	}
	namespace vNode
	{
		const path
			drawableBlock{ "21"_np },
			frame{ drawableBlock / draBlock::frame },
			position{ frame / rect::position },
			size{ frame / rect::size },
			node{ "0"_np },
			vConnectionGroup{ "1"_np },
			nodeText{ "211"_np },
			nodeBoard{ "3333"_np },
			vGroup{ "3133"_np };
		constexpr serialPlan getPlan()
		{
			return
			{
				{L"vNode",
					{
						{nullptr},
						{L"vConnectionGroup", sType::weakGroup()},
						{L"attributes",
							{
								{nullptr},
								{L"draBlock", draBlock::getPlan()},
								{nullptr}
							}}
					}},
				{nullptr},
				{nullptr}
			};
		}
		pointer newConnectionPort(nodePtr vNode)
		{
			assert(groupPtr::match(vConnectionGroup / vNode));
			return backGroupPort(vConnectionGroup / vNode);
		}
		bool vBlockIsNode(nodePtr vBlock)
		{
			nodePtr group = "33"_np / vBlock;//������ �� nBoard
			return group.reverseLocalPos(3) == 0;
			//�������� ��������� ������ (vNodeGroup ��� ���)
		}
	}
	namespace vConnection
	{
		const path
			vNode0{ "003"_np },
			vNode1{ "103"_np },
			verticle0{ /*"210"_np*/ "032100"_np },
			verticle1{ /*"211"_np*/ "132100"_np },
			color{ "221"_np },
			thinkness{ "220"_np },
			nodeBoard{ "3333"_np },
			vGroup{ "3133"_np };
		constexpr serialPlan getPlan()
		{
			return
			{
				{L"vConnection",
				{
					{nullptr},
					{nullptr},
					{L"attributes", 
						{
							{L"type", sType::i64v(0)},
							{L"verticleGroup", sType::strongGroup()},//���� �����
							{L"visual",
								{
									{L"thinkness", sType::f64v(5.f)},
									{L"color", sType::i64v(0)}
								}}
						}}
				}}
			};
		}
		bool isVConnection(nodePtr vBlock)
		{
			nodePtr group = "33"_np / vBlock;//������ �� nBoard
			return group.reverseLocalPos(3) == 1;
			//�������� ��������� ������ (vConnectionGroup ��� ���)
		}
	}
	namespace vGroup
	{
		const path
			vBlockGroup{ "00"_np },//vNode � vGroup
			vConnectionGroup{ "01"_np },
			drawableBlock{ "21"_np },
			frame{ drawableBlock / draBlock::frame },
			type{ "200"_np },
			data{ "201"_np },
			nodeBoard{ "3333"_np },
			vGroup{ "3133"_np };
		enum class gType
		{
			simple, //������� ��������������� ������
			consistent, //������������� ������. ������ ��� �������
			round, //��� �� consistent, ������ �� ����� � ����������� �������
			tree, //������
			roundTree, //�������� ������. ����� ���������� � ����� �� �����
			code, //������ ��� ������������ ����
			flowGr, //��������� ������. ���� ����� ����������� ����
			another //���� ��������� ����������� �����
		};
		namespace consistent
		{
			const path
				horisontal{ "00"_np },
				rightAlignment{ "01"_np },
				reverseDirection{ "02"_np },
				frameGroup{ "1"_np };
			constexpr serialPlan getPlan()
			{
				return
				{
					{L"settings",
					{
						{L"horisontal", sType::i64v(0)},
						{L"rightAlignment", sType::i64v(0)},
						{L"reverseDirection", sType::i64v(0)}
					}},
					{L"frameGroup", sType::weakGroup()}
				};
			}
		}
		constexpr serialPlan getPlan(gType type = gType::simple)
		{
			return
			{
				{L"vGroup",
				{
					{L"content",
					{
						{L"vBlockGroup", sType::weakGroup()},//vNode � vGroup
						{L"vConnectionGroup", sType::weakGroup()}
					}},
					{nullptr},
					{L"attributes",
					{
						{L"type",
						{
							{L"typeId", sType::i64v(static_cast<i64>(type))},
							{L"typeData", (type == gType::consistent) ?
							consistent::getPlan() : nullptr}
						}},
						{L"draBlock", draBlock::getPlan()}
					}}
				}}
			};
		}

		//�����������, �������� �� vBlock �������
		bool vBlockIsGroup(nodePtr vBlock)
		{
			nodePtr group = "33"_np / vBlock;//������ �� nBoard
			return group.reverseLocalPos(3) == 2; 
			//�������� ��������� ������ (vGroupGroup ��� ���)
		}
	}
	namespace nBoard
	{
		const path
			vNodeGroup{ "00"_np },
			vConnectionGroup{ "01"_np },
			vGroupGroup{ "02"_np };
		constexpr serialPlan getPlan()
		{
			return
			{
				{L"content",
				{
					{L"vNodeGroup", sType::strongGroup()},
					{L"vConnectionGroup", sType::strongGroup()},
					{L"vGroupGroup", sType::strongGroup()}
				}}
			};
		}
	}
	namespace window
	{
		const path
			workBoard{ "00"_np },
			interfaceBoard{ "01"_np },
			font{ "1"_np };//�������� �� ��������

		constexpr serialPlan getPlan()
		{
			return
			{
				{L"activeBoard",
				{
					{L"workBoard", nBoard::getPlan()},
					{L"interfaceBoard", nBoard::getPlan()}
				}}
			};
		}
	}
	namespace nechtoIde
	{
		const path
			mouse{ "000"_np },
			keyboard{ "001"_np },
			services{ "01"_np },
			windowGroup{ "02"_np };

		constexpr serialPlan getPlan()
		{
			return
			{
				{L"interface",
				{
					{nullptr},
					{L"services", {{nullptr}, {nullptr}, {nullptr}}},//��������� ���������� �������
					{L"windowGroup", sType::weakGroup()}
				}}
			};
		}
	}
	namespace uh
	{
		const path
			input{ "0"_np },
			keyboard{ "00"_np },
			mouse{ "01"_np },
			selectH{ "100"_np },
			shiftingH{ "101"_np },
			interactiveH{ "110"_np },
			dropBlockH{ "111"_np },
			textField{ "112"_np };
		constexpr serialPlan getPlan()
		{
			return
			{
				{L"inputDevices",
				{
					{nullptr},//keyBoard
					{nullptr},//mouse
				}},
				{L"inputInstruments",
				{
					{L"workBoard",
					{
						{nullptr},//selectH,
						{nullptr}//shiftingH
					}},
					{L"interfaceBoard",
					{
						{nullptr},//interactiveH
						{nullptr},//dropBlockH
						{nullptr}//textField
					}}
				}}
			};
		}
	}
	namespace mouse
	{
		enum class workMode
		{
			free,//���������
			frameSelection,//��������� ������
			nodeMoving,//����������� ����
			interactionWElement//�������������� � ������������� ���������
		};
		const path
			current{ "00"_np },
			curBoard{ "000"_np },//������� nodeBoard
			curVel{ "001"_np },//������� visualElement
			winPos{ "010"_np },//������� ������������ ����
			velPos{ "011"_np },//������� ������������ �������� ��������
			leftButton{ "1000"_np },
			rightButton{ "1001"_np },
			x1Button{ "1010"_np },
			x2Button{ "1011"_np },
			middleButton{ "1020"_np },
			wheel{ "1021"_np },
			workMode{ "110"_np },
			clickStartPoint{ "111"_np },
			selectManager{ "120"_np },//�������� ����� ������
			textInputField{ "121"_np },//�������� ����� ������
			mouseLog{ "122"_np };//�������� ����� ������

		constexpr serialPlan getPlan()
		{
			return
			{
				{L"position",
				{
					{L"current",
					{
						{nullptr},//nodeBoard
						{nullptr},//visualElement//�������� ���������� �������� ������������
					}},
					{L"coordinates",
					{
						{L"window", vec2::getPlan()},
						{L"vel", vec2::getPlan()}
					}},
				}},
				{L"status",
				{
					{L"button",
					{
						{L"general",
						{
							{L"left", sType::i64v(0)},
							{L"right", sType::i64v(0)},
						}},
						{L"additional",
						{
							{L"x1", sType::i64v(0)},
							{L"x2", sType::i64v(0)}
						}},
						{L"wheel",
						{
							{L"x1", sType::i64v(0)},
							{L"x2", sType::i64v(0)}
						}}
					}},
					{L"workMode",
					{
						{L"id", sType::i64v(0)},
						{L"clickStartPos", vec2::getPlan()}
					}},
					{L"tools", //�������� ����� ��������
					{
						{nullptr},//selectManager
						{nullptr},//textInputField
						{nullptr}//mouseLog
					}}
				}}
			};
		}
	}
	namespace event
	{
		const path
			type("00"_np),
			data("01"_np),
			subscriptionGroup("1"_np),
			source("2"_np),
			eventStorage("3"_np);
		constexpr serialPlan getPlan()
		{
			return
			{
				{L"data", sType::structNode(0, structData())},
				{L"subscriptionGroup", sType::weakGroup()}
			};
		}
	}
	namespace eventSource
	{
		const path
			eventGroup("0"_np),
			subscriptionGroup("1"_np),
			parentGroup("20"_np),
			childrenGroup("21"_np);

		constexpr serialPlan getPlan()
		{
			return
			{
				{L"eventGroup", sType::weakGroup()},
				{L"subscriptionGroup", sType::weakGroup()},
				{L"tree",
				{
					{L"parentGroup", sType::weakGroup()},
					{L"childrentGroup", sType::weakGroup()}
				}}
			};
		}
	}
	namespace eventType
	{
		const path
			eventGroup("0"_np),
			subscriptionGroup("1"_np),
			parentGroup("20"_np),
			childrenGroup("21"_np);

		constexpr serialPlan getPlan()
		{
			return
			{
				{L"eventGroup", sType::weakGroup()},
				{L"subscriptionGroup", sType::weakGroup()},
				{L"tree",
				{
					{L"parentGroup", sType::weakGroup()},
					{L"childrentGroup", sType::weakGroup()}
				}}
			};
		}
	}
	namespace eventSubscription
	{
		const path
			nonHandledEvent("00"_np),
			handlingNowEvent("01"_np),//��������� ������������� �� ���-�� ������������
			handledEvent("02"_np),
			parameters("1"_np),
			source("10"_np),//�������� �������
			type("11"_np);//��� �������

		constexpr serialPlan getPlan()
		{
			return
			{
				{L"eventGroup",
				{
					{L"nonHandledEvent", sType::weakGroup()},
					{L"handlingNowEvent", sType::weakGroup()},
					{L"handledEvent", sType::weakGroup()}
				}},
				{L"parametrs",
				{
					{nullptr},//��������
					{nullptr}//��� �������
				}}
			};
		}
	}
	
}
