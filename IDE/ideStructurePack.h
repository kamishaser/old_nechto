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
			charSize{ "221"_np };
			//illuminationThinkness{ "222"_np };
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
		bool match(nodePtr node)//минимально подходит под определение
		{//что не подходит - по умолчанию
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
			nodePtr group = "33"_np / vBlock;//группа от nBoard
			return group.reverseLocalPos(3) == 0;
			//проверка обратного адреса (vNodeGroup или нет)
		}
	}
	namespace vConnection
	{
		const path
			vNode0{ "03"_np },
			vNode1{ "13"_np },
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
							{L"verticleGroup", sType::strongGroup()},//пока пусто
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
			nodePtr group = "33"_np / vBlock;//группа от nBoard
			return group.reverseLocalPos(3) == 1;
			//проверка обратного адреса (vConnectionGroup или нет)
		}
	}
	namespace vGroup
	{
		const path
			vBlockGroup{ "00"_np },//vNode и vGroup
			vConnectionGroup{ "01"_np },
			drawableBlock{ "21"_np },
			frame{ drawableBlock / draBlock::frame },
			type{ "200"_np },
			data{ "201"_np },
			nodeBoard{ "3333"_np },
			vGroup{ "3133"_np };
		enum class gType
		{
			simple, //простая неупорядоченная группа
			consistent, //упорядоченная группа. Строка или столбец
			round, //тот же consistent, только на круге с определённым центром
			tree, //дерево
			roundTree, //круговое дерево. Очень специфично и очень не скоро
			code, //группа для расположения кода
			flowGr, //плавающая группа. Тоже жудко специфичная вещь
			another //буду добавлять значительно позже
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
						{L"vBlockGroup", sType::weakGroup()},//vNode и vGroup
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

		//определение, является ли vBlock группой
		bool vBlockIsGroup(nodePtr vBlock)
		{
			nodePtr group = "33"_np / vBlock;//группа от nBoard
			return group.reverseLocalPos(3) == 2; 
			//проверка обратного адреса (vGroupGroup или нет)
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
	namespace textMark
	{
		const path
			type{ "0"_np },//тип drawableEntity
			vNode{ "10"_np },
			beginNumber{ "11"_np },
			endNumber{ "12"_np },
			data{ "2"_np };//данные в зависимости от типа
		constexpr serialPlan getPlan(serialPlan data = serialPlan(nullptr))
		{
			return
			{
				{nullptr},//type
				{L"position",
				{
					{nullptr},//vNode
					{L"beginNumber", sType::i64v(0)},
					{L"endNumber", sType::i64v(0)}
				}},
				{nullptr}//data
			};
		}

	}
	namespace window
	{
		const path
			workBoard{ "00"_np },
			interfaceBoard{ "01"_np },
			font{ "1"_np };//временно не работает

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
			display{ "010"_np },
			workZone{ "0100"_np },
			workZoneContentHdl{ "01000"_np },
			nodePositionHdl{ "01001"_np },
			interfaceHdl{ "0101"_np },
			interfaceTreeHdl{ "01010"_np },
			interactiveHandler{ "01011"_np },
			general{ "0102"_np },
			textHandler{ "01020"_np },
			illuminationHandler{ "01021"_np },
			baseInput{ "011"_np },
			selectHanlder{ "0110"_np },
			textInputField{ "0111"_np },
			nodeMoveHandler{ "0112"_np },
			userH{ "012"_np },
			windowGroup{ "02"_np };

		constexpr serialPlan getPlan()
		{
			return
			{
				{L"interface",
				{
					{L"devices", sType::structNode(0, structData())},
					{L"services", 
					{
						{L"display",
						{
							{L"workZone", 
							{
								{nullptr},//обработчик контента
								{L"nodePositionHdl", {{nullptr}, {nullptr}}},
							}},
							{L"interface",
							{
								{nullptr},//interfaceTree
								{nullptr},//interactiveElTree
							}},
							{L"general",
							{
								{nullptr},//textHandler
								{nullptr},//illuminationHanlder

							}}
						}},
						{L"baseInput",
						{
							{nullptr},//selectHandler
							{nullptr},//textInpurField
							{nullptr}//nodeMoveHandler
						}},
						{nullptr}//userH
					}},//различные встроенные сервисы
					{L"windowGroup", sType::weakGroup()},
				}}
			};
		}
	}
	namespace event
	{
		const path
			type("00"_np),
			content("01"_np),
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
			handlingNowEvent("01"_np),//требуется переименовать во что-то удобоваримое
			handledEvent("02"_np),
			parameters("1"_np),
			source("10"_np),//источник события
			type("11"_np);//тип события

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
					{nullptr},//источник
					{nullptr}//тип события
				}}
			};
		}
	}
	namespace mouse
	{
		enum class workMode
		{
			free,//свободный
			frameSelection,//выделение рамкой
			nodeMoving,//перемещение ноды
			interactionWElement//взаимодействие с интерактивным элементом
		};
		const path
			current{ "00"_np },
			curBoard{ "000"_np },//текущий nodeBoard
			curVel{ "001"_np },//текущий visualElement
			winPos{ "010"_np },//позиция относительно окна
			velPos{ "011"_np },//позиция относительно текущего элемента
			leftButton{ "1000"_np },
			rightButton{ "1001"_np },
			x1Button{ "1010"_np },
			x2Button{ "1011"_np },
			middleButton{ "1020"_np },
			wheel{ "1021"_np },
			pressMoveMode{ "110"_np },
			clickStartPoint{ "111"_np },
			eventType_illuminationChange{ "120"_np },//источник события изменения выделения
			eventSource{ "20"_np },
			eventType_OveredChanged{ "2100"_np },
			eventType_WindowChanged{ "2101"_np },
			eventType_ButtonClicked{ "2110"_np },
			eventType_WheelMoved{ "2111"_np },
			eventType_PressMoveMode{ "2112"_np };

		constexpr serialPlan getPlan()
		{
			return
			{
				{L"position",
				{
					{L"current",
					{
						{nullptr},//nodeBoard
						{nullptr},//visualElement//механизм реализации временно отстутствует
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
						{L"pressMoveMode", sType::i64v(0)},
						{L"clickStartPos", vec2::getPlan()}
					}},
					{L"tools", //название стоит поменять
					{
						{nullptr},//selectManager
						{nullptr},//textInputField
						{nullptr}//mouseLog
					}}
				}},
				{L"events",
				{
					{L"source", eventSource::getPlan()},
					{L"types",
					{
						{L"move",
						{
							{L"mOverElChanged", eventType::getPlan()},
							{L"windowChanged", eventType::getPlan()}
						}},
						{L"buttons",
						{
							{L"buttonClicked", eventType::getPlan()},
							{L"wheelMoved", eventType::getPlan()},
							{L"pressMoveMode", eventType::getPlan()}
						}}
					}}
				}}
			};
		}
	}
	namespace keyboard
	{
		
	}
	namespace illuminationHandler
	{
		const path
			warningGroup{ "000"_np },
			errorGroup{ "001"_np },
			mouseCurVel{ "002"_np },
			tip0Group{ "010"_np },
			tip1Group{ "011"_np },
			tip2Group{ "012"_np },
			selectGroup{ "02"_np },
			eventType_illGExchenge{ "10"_np },
			mOverSubcription{ "11"_np },
			illGExchengeSubscription{ "12"_np };
		const serialPlan getPlan()
		{
			return
			{
				{L"illuminElem",
				{
					{L"weR",
					{
						{L"warningGroup", sType::weakGroup()},
						{L"errorGroup", sType::weakGroup()}
					}},
					{L"tips",
					{
						{L"tip0", sType::weakGroup()},
						{L"tip1", sType::weakGroup()},
						{L"tip2", sType::weakGroup()}
					}},
					{L"selectGroup", sType::weakGroup()}
				}},
				{L"events",
				{
					{L"illGExchengeEventType", sPack::eventType::getPlan()},
					{L"mOverChangeSubscription", sPack::eventSubscription::getPlan()},
					{L"illGExchengeSubscription", sPack::eventSubscription::getPlan()}
				}}
			};
		}
	}
	namespace selectHandler
	{
		const path
			selectGroup("00"_np),
			eventType_illGExchenge{ "01"_np },
			eventSource{ "02"_np },
			mClickSubscription{ "10"_np },
			mouse{ "11"_np };
		constexpr serialPlan getPlan()
		{
			return
			{
				{L"illHdl",
				{
					{nullptr},//selectGroup
					{nullptr},//eventType
					{L"eventSource", eventSource::getPlan()}//eventSource
				}},
				{L"mouse",
				{
					{L"subscription", eventSubscription::getPlan()},//подписка на клик
					{nullptr}//статус левой кнопки мыши
				}}
			};
		}
	}
	namespace interactiveHandler
	{
		const path
			mOverChSubscription{ "00"_np },//подписка на наведение
			mClickSubscription{ "01"_np },	//подписка на клик
			et_mOver{ "10"_np },//тип события: наведения мыши
			et_lClick{ "110"_np },
			et_rClick{ "111"_np },
			et_mClick{ "112"_np },
			mouse{ "20"_np },
			curIElEventSource{ "211"_np },
			currentIEl{ "213"_np };//текущий интеркативный элемент

		constexpr serialPlan getPlan()
		{
			return
			{
				{L"subscriptions",
				{
					{L"mOverChSubscription", sPack::eventSubscription::getPlan()},
					{L"clickSubscription", sPack::eventSubscription::getPlan()}
				}},
				{L"eventTypes",
				{
					{L"et_mOver", sPack::eventType::getPlan()},
					{L"click",
					{
						{L"et_lClick", sPack::eventType::getPlan()},
						{L"et_rClick", sPack::eventType::getPlan()},
						{L"et_mClick", sPack::eventType::getPlan()}
					}}
				}},
				{L"other",
				{
					{nullptr},//mouse
					{nullptr},//curentInteractiveElement
				}}
			};
		}
	}
	namespace interEl//интерактивный элемент
	{
		const path
			interactiveHandler{ "003"_np },//сработает только если элемент активен
			mouse{ "000"_np },//сработает только если элемент активен
			eventSource{ "01"_np },
			type{ "02"_np },
			data{ "1"_np },
			vNode{ "2"_np };
		constexpr serialPlan getPlan(serialPlan data = serialPlan{ nullptr })
		{
			return
			{
				{L"general",
				{
					{nullptr},//interactiveHandler
					{L"eventSource", sPack::eventSource::getPlan()},
					{nullptr}//eventType
				}},
				{L"data", data},
				{nullptr}//vNode
			};
		}

	}
	namespace textInputField
	{
	}

	namespace uh
	{
		//const path
		//	input{ "0"_np },
		//	keyboard{ "00"_np },
		//	mouse{ "01"_np },
		//	selectH{ "100"_np },
		//	shiftingH{ "101"_np },
		//	interactiveH{ "110"_np },
		//	dropBlockH{ "111"_np },
		//	textField{ "112"_np };
		//constexpr serialPlan getPlan()
		//{
		//	return
		//	{
		//		{L"inputDevices",
		//		{
		//			{nullptr},//keyBoard
		//			{nullptr},//mouse
		//		}},
		//		{L"inputInstruments",
		//		{
		//			{L"workBoard",
		//			{
		//				{nullptr},//selectH,
		//				{nullptr}//shiftingH
		//			}},
		//			{L"interfaceBoard",
		//			{
		//				{nullptr},//interactiveH
		//				{nullptr},//dropBlockH
		//				{nullptr}//textField
		//			}}
		//		}}
		//	};
		//}
	}
	
}
