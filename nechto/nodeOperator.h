#pragma once
#include "connectionIterator.h"
#include "connectionRule.h"

#include "externalObject.h"
#include "nodeOperationSet.h"
#include "method.h"
#include "groupPtr.h"
#include "text.h"

namespace nechto
{
	const staticNodeOperationSet nodeOperator
	{
		//основные проверки
		namedOperation(L"exist", operation {
			connectionRule(conRule::I64Variable, conRule::Output, nullptr,
				conRule::AnyPointer_NoTransit, conRule::Input),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				if (pointer::follow(v1).exist())
					v0->setData<i64>(1);
				else
					v0->setData<i64>(0);
				return true;
			}}),
		namedOperation(L"getType",operation{
			connectionRule(conRule::I64Variable, conRule::Output, nullptr,
				conRule::AnyPointer, conRule::Input),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				v0->setData<i64>(v1->getType());
				return true;
			}}),
		namedOperation(L"getSubtype",operation{
			connectionRule(conRule::I64Variable, conRule::Output, nullptr,
				conRule::AnyPointer, conRule::Input),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				v0->setData<i64>(v1->getSubtype());
				return true;
			}}),
		namedOperation(L"typeCompare",operation{
			connectionRule(conRule::I64Variable, conRule::Output, nullptr,
				conRule::AnyPointer, conRule::Input, nullptr,
				conRule::AnyPointer, conRule::Input),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				if (v1->getType() == v2->getType())
					v0->setData<i64>(1);
				else
					v0->setData<i64>(0);
				return true;
			}}),
		namedOperation(L"typeSubtypeCompare",operation{
			connectionRule(conRule::I64Variable, conRule::Output, nullptr,
				conRule::AnyPointer, conRule::Input, nullptr,
				conRule::AnyPointer, conRule::Input),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				if (v1->getType() == v2->getType() && v1->getSubtype() == v2->getSubtype())
					v0->setData<i64>(1);
				else
					v0->setData<i64>(0);
				return true;
			}}),
		namedOperation(L"checkTheCorrectness",operation{
			connectionRule(conRule::I64Variable, conRule::Output, nullptr,
				conRule::AnyPointer, conRule::Input),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				if (check(v1))
					v0->setData<i64>(1);
				else
					v0->setData<i64>(0);
				return true;
			}}),
		namedOperation(L"hasHub",operation{
			connectionRule(conRule::I64Variable, conRule::Output, nullptr,
				conRule::AnyPointer, conRule::Input),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				if (v1->hasHub())
					v0->setData<i64>(1);
				else
					v0->setData<i64>(0);
				return true;
			}}),
		namedOperation(L"numberOfExistingConnections",operation{
			connectionRule(conRule::I64Variable, conRule::Output, nullptr,
				conRule::AnyPointer, conRule::Input),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				int counter = 0;
				connectionIterator i(v1);
				do
				{
					if (i.get().exist())
						++counter;
				} while (i.stepForward());
				v0->setData<i64>(counter);
				return true;
			}}),
		namedOperation(L"numberOfHubInGroup",operation{
			connectionRule(conRule::I64Variable, conRule::Output, nullptr,
				conRule::Group, conRule::Input),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				int counter = 0;
				groupIterator i(v1);
				do
				{
					++counter;
				} while (i.GoToNextHub());
				if (v0.exist())
					v1->setData<i64>(1);
				else
					v1->setData<i64>(0);
				return true;
			}}),
		namedOperation(L"numberOfGroupMembers",operation{
			connectionRule(conRule::I64Variable, conRule::Output, nullptr,
				conRule::Group, conRule::Input),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				v0->setData<i64>(group::numberOfMembers(v1));
				return true;
			} }),
		namedOperation(L"exObjExist",operation{
			connectionRule(conRule::I64Variable, conRule::Output, nullptr,
				conRule::ExternalObject, conRule::Input),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				if (v1->getData<externalObject*>() != nullptr)
					v0->setData<i64>(1);
				else
					v0->setData<i64>(0);
				return true;
			} }),
		namedOperation(L"textSize", operation{
			connectionRule(conRule::I64Variable, conRule::Output, nullptr,
				conRule::Text, conRule::Input),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				v0->setData<i64>(text::get(v1).size());
				return true;
			}}),
		namedOperation(L"textsEqual",operation{
			connectionRule(conRule::I64Variable, conRule::Output, nullptr,
				conRule::Text, conRule::Input, nullptr,
				conRule::Text, conRule::Input),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				if (text::get(v1) == text::get(v2))
					v0->setData<i64>(1);
				else
					v0->setData<i64>(0);
				return true;
			} }),
		namedOperation(L"compressGroup",operation{
			connectionRule(conRule::Group, conRule::In_Output),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				group::compress(v0);
				return true;
			} }),
			//перемещение итератора
		namedOperation(L"GoToNextHub",operation{
			connectionRule(conRule::AnyIterator_NoTransit, conRule::In_Output, nullptr,
				conRule::I64Variable, conRule::Output),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				if (v0->getSubtype() == pointer::ConIter)
				{
					connectionIterator i(v0->connection[0], v0->getData<pointer::hubPosPair>());
					v1->setData<i64>(i.GoToNextHub());
					i.push(v0);
				}
				else
				{
					groupIterator i(v0->connection[0], v0->getData<pointer::hubPosPair>());
					v1->setData<i64>(i.GoToNextHub());
					i.push(v0);
				}
				return true;
			} }),
		namedOperation(L"GoToPreviousHub",operation{
			connectionRule(conRule::GroupIterator_NoTransit, conRule::In_Output, nullptr,
				conRule::I64Variable, conRule::Output),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				groupIterator i(v0->connection[0], v0->getData<pointer::hubPosPair>());
					v1->setData<i64>(i.GoToPreviousHub());
					i.push(v0);
				return true;
			} }),
		namedOperation(L"stepForward",operation{
			connectionRule(conRule::AnyIterator_NoTransit, conRule::In_Output, nullptr,
				conRule::I64Variable, conRule::Output),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				if (v0->getSubtype() == pointer::ConIter)
				{
					connectionIterator i(v0->connection[0], v0->getData<pointer::hubPosPair>());
					v1->setData<i64>(i.stepForward());
					i.push(v0);
				}
				else
				{
					groupIterator i(v0->connection[0], v0->getData<pointer::hubPosPair>());
					v1->setData<i64>(i.stepForward());
					i.push(v0);
				}
				return true;
			} }),
		namedOperation(L"stepBack",operation{
			connectionRule(conRule::GroupIterator_NoTransit, conRule::In_Output, nullptr,
				conRule::I64Variable, conRule::Output),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				groupIterator i(v0->connection[0], v0->getData<pointer::hubPosPair>());
					v1->setData<i64>(i.stepBack());
					i.push(v0);
				return true;
			} }),
		namedOperation(L"insertHub",operation{
			connectionRule(conRule::AnyIterator_NoTransit, conRule::In_Output),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				if (v0->getSubtype() == pointer::ConIter)
				{
					connectionIterator i(v0->connection[0], v0->getData<pointer::hubPosPair>());
					i.insertHub();
					i.push(v0);
				}
				else
				{
					groupIterator i(v0->connection[0], v0->getData<pointer::hubPosPair>());
					i.insertHub();
					i.push(v0);
				}
				return true;
			} }),
		namedOperation(L"eraseHub",operation{
			connectionRule(conRule::AnyIterator_NoTransit, conRule::In_Output),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				if (v0->getSubtype() == pointer::ConIter)
				{
					connectionIterator i(v0->connection[0], v0->getData<pointer::hubPosPair>());
					i.eraseHub();
					i.push(v0);
				}
				else
				{
					groupIterator i(v0->connection[0], v0->getData<pointer::hubPosPair>());
					i.eraseHub();
					i.push(v0);
				}
				return true;
			} }),
		namedOperation(L"atFirstHub",operation{
			connectionRule(conRule::AnyIterator_NoTransit, conRule::In_Output, nullptr,
				conRule::I64Variable, conRule::Output),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				if (v0->getSubtype() == pointer::ConIter)
				{
					connectionIterator i(v0->connection[0], v0->getData<pointer::hubPosPair>());
					v1->setData<i64>(i.currentHub == i.mainNode);
				}
				else
				{
					groupIterator i(v0->connection[0], v0->getData<pointer::hubPosPair>());
					v1->setData<i64>(i.atFirstHub());
				}
				return true;
			} }),
		//добавить перемещение в определённуюю локальную позицию
		
			//отключение
		namedOperation(L"I_Disconnect",operation{
			connectionRule(conRule::AnyIterator_NoTransit, conRule::In_Output),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				hubIterator i(v0->connection[0], v0->getData<pointer::hubPosPair>());
				i.disconnect();
				return true;
			} }),	
		namedOperation(L"HH_disconnect",operation{
			connectionRule(conRule::AnyPointer, conRule::In_Output, nullptr,
				conRule::AnyPointer, conRule::In_Output),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				nechto::disconnect(v0, v1);
				return true;
			} }),
			//подключение
		namedOperation(L"II_Connect",operation{
			connectionRule(conRule::AnyIterator_NoTransit, conRule::In_Output, nullptr,
				conRule::AnyIterator_NoTransit, conRule::In_Output),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				hubIterator i0(v0->connection[0], v0->getData<pointer::hubPosPair>());
				hubIterator i1(v1->connection[0], v1->getData<pointer::hubPosPair>());
				IterIterConnect(i0, i1);
				return true;
			} }),
		namedOperation(L"IH_Connect",operation{
			connectionRule(conRule::AnyIterator_NoTransit, conRule::In_Output, nullptr,
				conRule::AnyPointer, conRule::In_Output),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				hubIterator i0(v0->connection[0], v0->getData<pointer::hubPosPair>());
				IterHubConnect(i0, v1);
				return true;
			} }),
		namedOperation(L"HH_Connect",operation{
			connectionRule(conRule::AnyPointer, conRule::In_Output, nullptr,
				conRule::AnyPointer, conRule::In_Output),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				HubHubConnect(v0, v1);
				return true;
			} }),
		namedOperation(L"swapConnections",operation{
			connectionRule(conRule::AnyIterator_NoTransit, conRule::In_Output, nullptr,
				conRule::AnyIterator_NoTransit, conRule::In_Output),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				hubIterator i0(v0->connection[0], v0->getData<pointer::hubPosPair>());
				hubIterator i1(v1->connection[0], v1->getData<pointer::hubPosPair>());
				return hubIterator::exchangeConnections(i0, i1);
			} }),
		//добавить подключение в первый свободный порт или конец группы
		
			//установка указателей и итераторов
		namedOperation(L"setIterator",operation{
			connectionRule(conRule::AnyIterator_NoTransit, conRule::Output, nullptr,
				conRule::AnyPointer, conRule::Input),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				if (v0->getSubtype() == pointer::ConIter)
				{
					connectionIterator i(v1);
					i.push(v0);
				}
				else
				{
					groupIterator i(v1);
					i.push(v0);
				}
				return true;
			} }),
		namedOperation(L"setPointer",operation{
			connectionRule(conRule::SimplePointer_NoTransit, conRule::Output, nullptr,
				conRule::AnyPointer_NoTransit, conRule::Input),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				nodePtr temp = pointer::follow(v1);
				if (!temp.exist())
					numDisconnect(v0, 0);
				else
					NumHubConnect(v1, temp, 0);
				return true;
			} }),
		namedOperation(L"getMainNode",operation{
			connectionRule(conRule::AnyPointer_NoTransit, conRule::Output, nullptr,
				conRule::AnyIterator_NoTransit, conRule::Input),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				assert(false);//pointer::set()
				return true;
			} }),
		namedOperation(L"copyIterator",operation{
			connectionRule(conRule::AnyIterator_NoTransit, conRule::Output, nullptr,
				conRule::AnyIterator_NoTransit, conRule::Input),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				hubIterator i1(v1->connection[0], v1->getData<pointer::hubPosPair>());
				i1.push(v0);
				return true;
			} }),
			//получение позиции итератора
		namedOperation(L"globalPosition",operation{
			connectionRule(conRule::I64Variable, conRule::Output, nullptr,
				conRule::AnyIterator_NoTransit, conRule::Input),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				if (v1->getSubtype() == pointer::ConIter)
				{
					connectionIterator position(v1->connection[1], v1->getData<pointer::hubPosPair>());
					i64 counter = 0;
					connectionIterator iter(position.mainNode);
					do
					{
						if (position.currentHub == iter.currentHub)
						{
							v0->setData<i64>(counter + position.pos());
							return true;
						}
						counter += 4;
					} while (iter.stepForward());
				}
				else
				{
					groupIterator position(v1->connection[1], v1->getData<pointer::hubPosPair>());
					i64 counter = 0;
					groupIterator iter(position.mainNode);
					do
					{
						if (position.currentHub == iter.currentHub)
						{
							v0->setData<i64>(counter + position.pos());
							return true;
						}
						counter += 4;
					} while (iter.stepForward());
				}
				return false;
			} }),
		namedOperation(L"localPosition",operation{
			connectionRule(conRule::I64Variable, conRule::Output, nullptr,
				conRule::AnyIterator_NoTransit, conRule::Input),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				v0->setData<i64>(v1->getData<pointer::hubPosPair>().second);
				return true;
			} }),
			//создание нод различных типов
		namedOperation(L"newVariable",operation{
			connectionRule(conRule::AnyPointer_NoTransit, conRule::In_Output, nullptr,
				conRule::I64Variable, conRule::Input),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				nodePtr temp;
				if (v1->getData<i64>())
				{
					temp = newNode(node::Variable, 1);
					temp->setData<i64>(0);
				}
				else
				{
					temp = newNode(node::Variable, 0);
					temp->setData<f64>(0.0);
				}
				pointer::set(v0, temp);
				return true;
			} }),
		namedOperation(L"newMathOperator",operation{
			connectionRule(conRule::AnyPointer_NoTransit, conRule::In_Output, nullptr,
				conRule::I64Variable, conRule::Input),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				nodePtr temp;
				temp = newNode(node::MathOperator, v1->getData<i64>());
				pointer::set(v0, temp);
				return true;
			} }),
		namedOperation(L"newConditionalBranching",operation{
			connectionRule(conRule::AnyPointer_NoTransit, conRule::In_Output),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				nodePtr temp;
				temp = newNode(node::ConditionalBranching);
				pointer::set(v0, temp);
				return true;
			} }),
		namedOperation(L"newMethod",operation{
			connectionRule(conRule::AnyPointer_NoTransit, conRule::In_Output, nullptr,
				conRule::I64Variable, conRule::Input),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				nodePtr temp;
				temp = newNode(node::Method, v1->getData<i64>());
				pointer::set(v0, temp);
				return true;
			} }),
		namedOperation(L"newExternalObject",operation{
			connectionRule(conRule::AnyPointer_NoTransit, conRule::In_Output),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				nodePtr temp;
				temp = newNode(node::ExternalObject);
				temp->setData<externalObject*>(nullptr);
				pointer::set(v0, temp);
				return true;
			} }),
		namedOperation(L"newText",operation{
			connectionRule(conRule::AnyPointer_NoTransit, conRule::In_Output),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				nodePtr temp;
				temp = newNode(node::Text);
				pointer::set(v0, temp);
				return true;
			} }),
		namedOperation(L"newPointer",operation{
			connectionRule(conRule::AnyPointer_NoTransit, conRule::In_Output, nullptr,
				conRule::I64Variable, conRule::Input, nullptr,
				conRule::AnyPointer, conRule::In_Output),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				nodePtr temp;
				if (v1->getData<i64>() == pointer::Simple)
				{
					temp = newNode(node::Pointer, pointer::Simple);
					NumHubConnect(temp, v2, 0);
					pointer::set(v0, temp);
				}
				else if
					(v1->getData<i64>() == pointer::ConIter)
				{
					temp = newNode(node::Pointer, pointer::ConIter);
					connectionIterator i(v2);
					i.push(temp);
					pointer::set(v0, temp);
				}
				else if
					(v1->getData<i64>() == pointer::GroupIter)
				{
					temp = newNode(node::Pointer, pointer::GroupIter);
					groupIterator i(v2);
					i.push(temp);
					pointer::set(v0, temp);
				}
				else
					return false;
				return true;
			} }),
		namedOperation(L"newGroup",operation{
			connectionRule(conRule::AnyPointer_NoTransit, conRule::In_Output),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				nodePtr temp;
				temp = newNode(node::Group);
				pointer::set(v0, temp);
				return true;
			} }),
		namedOperation(L"newNodeOperator",operation{
			connectionRule(conRule::AnyPointer_NoTransit, conRule::In_Output, nullptr,
				conRule::I64Variable, conRule::Input),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				nodePtr temp;
				temp = newNode(node::MathOperator, v1->getData<i64>());
				pointer::set(v0, temp);
				return true;
			} }),
			//дублирование
		namedOperation(L"duplicate",operation{
			connectionRule(conRule::AnyPointer_NoTransit, conRule::Output, nullptr,
				conRule::AnyPointer_NoTransit, conRule::Input),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				assert(false);//добавлено для галочки, но пока не работает
				return true;
			} }),
			//удаление ноды		
		namedOperation(L"deleteNode",operation{
				connectionRule(conRule::AnyPointer, conRule::In_Output),
			[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				nechto::deleteNode(v0);
				return true;
			} })
	};
	bool checkNodeOperator(nodePtr v1)
	{
		assert(typeCompare(v1, node::NodeOperator));
		return nodeOperator.getOperation(
			v1->getSubtype()).cRule.check(v1);
	}
	bool operateNodeOperator(nodePtr v1)
	{
		assert(typeCompare(v1, node::NodeOperator));
		return nodeOperator.getOperation(v1->getSubtype()).operate(v1);
	}
}
