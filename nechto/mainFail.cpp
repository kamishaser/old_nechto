#include "node.h"
#include "lowLevelGraphOperations.h"
#include "additionalCreationFunctions.h"
#include "baseValueTypes.h"
#include "typeNames.h"
#include "commandLine.h"
#include "tag.h"
#include "nechtoFileStream.h"

#include <iostream>
#include <exception>
#include <algorithm>

using namespace nechto;


int main()
{
	//externalFunction print
	//{
	//	//уникальное имя
	//	"print",
	//	//проверка корректности подключения
	//	[](nodePtr v1) {return v1->connectionType(0) == node::Variable; },
	//	//сама функция
	//	[](nodePtr v1) {std::cout << nodeData(v1->connection[0].load()) << std::endl; }
	//};
	//externalFunction intRead
	//{
	//	//уникальное имя
	//	"print",
	//	//проверка корректности подключения
	//	[](nodePtr v1) {return v1->connectionType(0) == node::Variable && v1->connectionSubtype(0); },
	//	//сама функция
	//	[](nodePtr v1) {std::cout << nodeData(v1->connection[0].load()) << std::endl; }
	//};
	//nodePtr printer = createFunction(&print);
	///*nodePtr v3 = createVariable<double>(3);
	//nodePtr v1 = createVariable<int>(3);
	//nodePtr v2 = createVariable<int>(2);
	//nodePtr result = createVariable<int>(0);
	//nodePtr Operator = createMathOperator(mathOperator::Subtraction, result, v1, v2);
	//nodePtr TypeCast = createTypeCastOperator(v3, v1);
	//nodePtr Operator2 = createMathOperator(mathOperator::Assigment, result, v1);
	//nodePtr Operator3 = createMathOperator(mathOperator::UnaryMinus, result, v1);
	//nodePtr Branching = createConditionalBranching(result, Operator2, Operator3);
	//serialConnection(Operator, Branching);
	//serialConnection(TypeCast, Operator);
	//serialConnection(Operator2, printer);
	//serialConnection(Operator3, printer);
	//NumHubConnect(printer, result, 0);
	//isCorrect(printer);*/

	//

	//nodePtr Variable = createVariable<int64_t>(0);
	//nodePtr Operator = createMathOperator(mathOperator::Increment, Variable);
	//serialConnection(Operator, Operator);
	//nodePtr temp = Operator;
	//for (int i = 0; i < 10; i++)
	//	step(temp);
	//std::cout << nodeData(Variable) << std::endl;
	//std::cout << to_string(temp) << std::endl;
	//commandLine c;
	//
	//while (true)
	//{
	//	try
	//	{
	//		std::string temp;
	//		std::getline(std::cin, temp);
	//		std::cin.clear();
	//		std::cout << c(temp) << std::endl;
	//	}
	//	catch (std::exception e)
	//	{
	//		std::cout << e.what() << std::endl;
	//	}
	//}

	int большаялажа = 5;
	std::cout << большаялажа << std::endl;
}