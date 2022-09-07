#pragma once
#include "node.h"
#include "connectionIterator.h"
#include "connectionRule.h"

#include "externalObject.h"
#include "method.h"
#include "group.h"
#include "text.h"

namespace nechto
{
	
	namespace nodeOperation
	{

		const operation typeCompare
		(
			
		);
		const operation typeSubtypeCompare
		(
			
		);
		const operation numberOfHubInGroup
		(
			
		);
		const operation numberOfExistingElementsInGroup
		(
			
		);
		const operation exObjExist
		(
			
		);
		const operation textSize
		(
			
		);
		const operation textsEqual
		(
			
		);
		const operation duplicate
		(
			
		);
		const operation deleteNode
		(
		
		);
		const operation compressGroup
		(
			
		);
		const operation HH_disconnect
		(
			
		);
		//////////////////////////////////////////////////////////////////////
		//операции с итерааторами
		const operation GoToNextHub
		(
			
		);
		const operation GoToPreviousHub
		(
			
		);
		const operation stepForward
		(
			
		);
		const operation stepBack
		(
			
		);
		const operation insertHub
		(
			
		);
		const operation eraseHub
		(
			
		);
		const operation atFirstHub
		(
			
		);
		const operation I_Disconnect
		(
			
		);
		const operation II_Connect
		(
			
		);
		const operation IH_Connect
		(
			
		);
		const operation HH_Connect
		(
			
		);
		const operation swapConnections
		(
			
		);
		const operation setIterator
		(
			
		);
		const operation setPointer
		(
			
		);

		const operation getMainNode
		(
			
		);
		const operation copyIterator
		(
			
		);

		const operation globalPosition
		(
			
		);
		const operation localPosition
		(
			
		);
		const operation newVariable
		(
			
		);
		const operation newMathOperator
		(
			
		);
		const operation newConditionalBranching
		(
			
		);
		const operation newMethod
		(
			
		);
		const operation newExternalObject
		(
			
		);
		const operation newText
		(
			
		);
		const operation newPointer
		(
			
		);
		const operation newGroup
		(
			
		);
		const operation newNodeOperator
		(
			
		);
	}
}
/*
операции с указател€ми (указатель на ноду по n0. ѕеременные по n1, n2 по необходимости):

измен€ющие соединени€:

создание ноды (отдельна€ функци€ дл€ каждого типа)
удаление,
оптимизаци€ хабов,



операции с итераторами:
не измен€ющие соединени€:

последовательное подключение по 2 указатл€м и переменной в случае условного ветвлени€

обмен мест подключени€ (по 2 итераторам)


*/