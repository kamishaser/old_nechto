#include "node.h"
#include "lowLevelGraphOperations.h"
#include "additionalCreationFunctions.h"
#include "baseValueTypes.h"
#include "typeNames.h"
#include "commandLine.h"

#include <iostream>
#include <exception>
#include <algorithm>

using namespace nechto;


int main()
{
	nodePtr v1 = createVariable<int>(2);
	nodePtr v2 = createVariable<int>(2);
	nodePtr result = createVariable<int>(0);
	nodePtr Operator = createMathOperator(mathOperator::Subtraction, result, v1, v2);

	nodePtr Operator2 = createMathOperator(mathOperator::Assigment, result, v1);
	nodePtr Operator3 = createMathOperator(mathOperator::UnaryMinus, result, v1);
	nodePtr Branching = createConditionalBranching(result, Operator2, Operator3);
	serialConnection(Operator, Branching);
	std::cout << to_string(Operator->connection[3]);
	while (true)
		if (!step(Operator))break;
	std::cout << nodeData(result) << std::endl;}
