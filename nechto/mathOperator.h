#pragma once
#include "node.h"
#include "pointer.h"

namespace nechto
{
	namespace mathOperator
	{
		struct operatorData
		{
			//������ ��������������� ���������.
			//�������� ������ �������� � ������� ������ ���������� � � ������ ���������
			//������ ������ ��� �� ��������� - �������� ������ ������������������
			//����� �������� ���������� �������� � ����������.
			//operatorData ������ � ����� ������ � ����������

			//true, ���� �������� �������� � ������� ������ ���� ��������
			bool isSimple;
			//type: f64 - false, i64 = true

			//IsDirect: true ���� ���������� ���������� ��������, 
			//false - ����� ���������

			struct connectionProperties
			{
				bool arguType;
				bool isDirect;
			};
			connectionProperties conProperties[3];

			//��������� ������
			bool fill(nodePtr v1, int numOfConnections)
			{
				//��������� true, ���� ���������� ���������
				for (int i = 0; i < numOfConnections+1; ++i)
				{
					nodePtr vcon = v1->connection[i].load();
					if (!vcon.exist())
						return false;
					else
					{
						if (vcon->getType() == node::Pointer)
						{
							conProperties[i].isDirect = false;
							vcon = pointer::follow(vcon);
							if (!vcon.exist())
								return false;
						}
						else
							conProperties[i].isDirect = true;
						if (vcon->getType() != node::Variable)
							return false;
						conProperties[i].arguType = vcon->getSubtype();
					}
				}
				setIsSimple();
				return true;
			}
			template<int number>
			bool intCon()
			{
				return conProperties[number].arguType;
			}
		private:
			void setIsSimple()
			{
				if (!conProperties[0].isDirect ||
					!conProperties[1].isDirect ||
					!conProperties[2].isDirect)
					isSimple = false;
				else if ((conProperties[0].arguType != conProperties[1].arguType) ||
					(conProperties[0].arguType != conProperties[2].arguType))
					isSimple = false;
				else isSimple = true;
			}
		};

		static_assert(sizeof(operatorData) <= 8);

		template<class TCon>
		void algebraicOperation(TCon* buffer, char operationType)
		{
			static_assert(sizeof(TCon) == 8);
			switch (operationType)
			{
			case Type::Assigment:
				buffer[0] = buffer[1];
				break;
			case Type::UnaryMinus:
				buffer[0] = -buffer[1];
				break;
			case Type::Addition:
				buffer[0] = buffer[1] + buffer[2];
				break;
			case Type::Subtraction:
				buffer[0] = buffer[1] - buffer[2];
				break;
			case Type::Multiplication:
				buffer[0] = buffer[1] * buffer[2];
				break;
			case Type::Division:
				buffer[0] = buffer[1] / buffer[2];
				break;
			}
		}
		template<class TCon>
		void comparisonOperation(i64* result, TCon* buffer, char operationType)
		{
			static_assert(sizeof(TCon) == 8);
			switch (operationType)
			{
			case Type::Equal:
				*result = buffer[0] == buffer[1];
				break;
			case Type::NotEqual:
				*result = buffer[0] != buffer[1];
				break;
			case Type::Less:
				*result = buffer[0] < buffer[1];
				break;
			case Type::Greater:
				*result = buffer[0] > buffer[1];
				break;
			case Type::LessOrEqual:
				*result = buffer[0] <= buffer[1];
				break;
			case Type::GreaterOrEqual:
				*result = buffer[0] >= buffer[1];
				break;
			default:
				assert(false);
			}
		}
		void logicOperation(i64* buffer, char operationType)
		{
			switch (operationType)
			{
			case Type::LogicNegation:
				buffer[0] = !buffer[1];
				break;
			case Type::LogicAnd:
				buffer[0] = buffer[1] && buffer[2];
				break;
			case Type::LogicOr:
				buffer[0] = buffer[1] || buffer[2];
				break;
			default:
				assert(false);
			}
		}
		void In_De_crement(i64* buffer, char operationType)
		{
			switch (operationType)
			{
			case Type::Increment:
				++buffer[0];
				break;
			case Type::Decrement:
				--buffer[0];
				break;
			default:
				assert(false);
			}
		}
		enum class prType
		{
			Algebraic,
			Comparison,
			Logic,
			In_De_crement
		};
		char numberOfArguments(char operType)
		{
			switch (operType)
			{
			case Increment:		// ++
				return 0;
				break;
			case Decrement:		// --
				return 0;
				break;
			
			case Assigment:		// =
				return 1;
			case UnaryMinus:	// 0-
				return 1;
			case LogicNegation:	// !
				return 1;
			default:
				return 2;
			}
		}
		prType operationPrType(char operType)
		{
			if (operType <= Type::Division)
				return prType::Algebraic;
			if (operType <= Type::GreaterOrEqual)
				return prType::Comparison;
			if (operType <= Type::LogicOr)
				return prType::Logic;
			return prType::In_De_crement;
		}
		//������� ����������. ������������, ����� ��� ������������� � �������������� ���� ������
		inline void simpleCompute(void* buffer, const operatorData opdat, prType prop, char operType)
		{
			//type: f64 - false, i64 = true
			if (opdat.conProperties[1].arguType)
			{
				switch (prop)
				{
				case prType::Algebraic:
					algebraicOperation<i64>
						(reinterpret_cast<i64*>(buffer), operType);
					break;
				case prType::Comparison:
					comparisonOperation<i64>
						(reinterpret_cast<i64*>(buffer), reinterpret_cast<i64*>(buffer) + 1, operType);
					break;
				case prType::Logic:
					logicOperation
						(reinterpret_cast<i64*>(buffer), operType);
					break;
				case prType::In_De_crement:
					In_De_crement(reinterpret_cast<i64*>(buffer), operType);
				}
			}
			else
			{
				if (prop == prType::Algebraic)
					algebraicOperation<f64>
					(reinterpret_cast<f64*>(buffer), operType);
				else
					comparisonOperation<f64>
					(reinterpret_cast<i64*>(buffer), reinterpret_cast<f64*>(buffer) + 1, operType);
			}
		}
		inline nodePtr getVariableByPointerOrDirect(nodePtr v1, operatorData opdata, char number)
		{
			if (opdata.conProperties[number].isDirect)
				return v1->connection[number].load();
			//��� ����������� ����� ��������� ���������� ������ ��� ��������� ��������
			nodePtr temp =
				v1->connection[number].load()->connection[0].load();
			if (!temp.exist())
				return temp;
			if (temp->getType() != node::Variable || 
				temp->getSubtype() != opdata.conProperties[number].arguType)
				return nullNodePtr;
			return temp;
		}
		inline void convert_f_to_i(void* address)
		{
			*static_cast<i64*>(address) =
				static_cast<i64>(*static_cast<f64*>(address));
		}
		inline void convert_i_to_f(void* address)
		{
			*static_cast<f64*>(address) =
				static_cast<f64>(*static_cast<i64*>(address));
		}
		inline void hardAlgebraic(void* buffer, const operatorData opdat, prType prop, char operType)
		{
			if (operType <= UnaryMinus) //������� ��������
				if (opdat.conProperties[1].arguType)
					algebraicOperation<i64>(
						static_cast<i64*>(buffer), operType);
				else
					algebraicOperation<f64>(
						static_cast<f64*>(buffer), operType);
			//�������� ��������
			//���� ��� ��������� ����� ��� i64, �� �������������� i64 ��������, ����� f64
			bool vType = opdat.conProperties[1].arguType && opdat.conProperties[2].arguType;
			if (vType)//i64
			{
				algebraicOperation<i64>(
					static_cast<i64*>(buffer), operType);
				//���� ��������� f64 (��� ���������� �� ������������� ���� ��������)
				if (!opdat.conProperties[0].arguType)
					convert_i_to_f(buffer);
			}
			else
			{
				//�� ����� ������ �� ���������� ����� ��� i64
				//���� ������ �������� ����� ��� i64
				if (opdat.conProperties[1].arguType)
					convert_i_to_f(&static_cast<i64*>(buffer)[1]);
				else if (opdat.conProperties[2].arguType)
					convert_i_to_f(&static_cast<i64*>(buffer)[2]);

				algebraicOperation<f64>(
					static_cast<f64*>(buffer), operType);
				//���� ��������� i64 (��� ���������� �� ������������� ���� ��������)
				if (opdat.conProperties[0].arguType)
					convert_f_to_i(buffer);
			}
		}
		inline void hardComparison(void* buffer, const operatorData opdat, prType prop, char operType)
		{
			bool vType = opdat.conProperties[1].arguType && opdat.conProperties[2].arguType;
			if (vType)//i64
			{
				comparisonOperation<i64>(
					static_cast<i64*>(buffer), static_cast<i64*>(buffer)+1, operType);
			}
			else
			{
				//�� ����� ������ �� ���������� ����� ��� i64
				//���� ������ �������� ����� ��� i64
				if (opdat.conProperties[1].arguType)
					convert_i_to_f(&static_cast<i64*>(buffer)[1]);
				else if (opdat.conProperties[2].arguType)
					convert_i_to_f(&static_cast<i64*>(buffer)[2]);

				comparisonOperation<i64>(
					static_cast<i64*>(buffer), (static_cast<i64*>(buffer))+1, operType);
			}
		}
		inline void hardCompute(void* buffer, const operatorData opdat, prType prop, char operType)
		{
			switch (prop)
			{
			case prType::Algebraic:
				hardAlgebraic(buffer, opdat, prop, operType);
				break;
			case prType::Comparison:
				hardComparison(buffer, opdat, prop, operType);
				break;
			case prType::Logic:
				logicOperation
				(reinterpret_cast<i64*>(buffer), operType);
				break;
			case prType::In_De_crement:
				In_De_crement(reinterpret_cast<i64*>(buffer), operType);
			}
		}
		bool perform(nodePtr v1)
		{
			try
			{
				operatorData opdat = v1->getData<operatorData>();
				size_t buffer[3];
				char operType = v1->getSubtype();
				char numOfArg = numberOfArguments(operType);
				if (opdat.isSimple)//������� �������� �������������� �������� (�����������)
				{
					switch (numOfArg)
					{
					case 0://0 ��� ������ ��������� � ���������
						buffer[0] = v1->connection[0].load()->getData<size_t>();
						break;
					case 1:
						buffer[1] = v1->connection[1].load()->getData<size_t>();
						break;
					case 2:
						buffer[1] = v1->connection[1].load()->getData<size_t>();
						buffer[2] = v1->connection[2].load()->getData<size_t>();
						break;
					}
					simpleCompute(buffer, opdat, operationPrType(operType), operType);
					v1->connection[0].load()->setData(buffer[0]);
					return true;
				}
				//���� �������� �������: � ��������������� ����� ���
				//������������ ���������
				//��������� �������� �������������� ������ ���

				/*��� ��� ���������� ����� ���� �������� �� ������ ��������,
				�� � ����� ��������� ���������� ������������� ����������
				������� ����������*/
				nodePtr connection[3];

				connection[0] = getVariableByPointerOrDirect(v1, opdat, 0);
				if (!connection[0].exist())
					return false;
				switch (numOfArg)
				{
				case 0://0 ��� ������ ��������� � ���������
					buffer[0] = connection[0]->getData<size_t>();
					break;
				case 2:
					connection[2] = getVariableByPointerOrDirect(v1, opdat, 2);
					if (!connection[2].exist())
						return false;
					buffer[2] = connection[2]->getData<size_t>();
				case 1:
					connection[1] = getVariableByPointerOrDirect(v1, opdat, 1);
					if (!connection[1].exist())
						return false;
					buffer[1] = connection[1]->getData<size_t>();
				}
				hardCompute(buffer, opdat, operationPrType(operType), operType);
				v1->connection[0].load()->setData(buffer[0]);
			}
			catch (...)
			{
				return false;
			}
			return true;
		}

		
		void reset(nodePtr v1) {}

		bool check(nodePtr v1)
		{
			char operType = v1->getSubtype();
			char numOfConnections = numberOfArguments(operType);
			
			operatorData opdat;
			if (!opdat.fill(v1, numOfConnections))
				return false;

			switch (operationPrType(operType))
			{
			case prType::Comparison:
			case prType::In_De_crement:
				if (!opdat.intCon<0>())//������ i64
					return false;
				break;
			case prType::Logic:
				if (!opdat.intCon<0>())//������ i64
					return false;
				if (!opdat.intCon<1>())//������ i64
					return false;
				if (!opdat.intCon<2>() && operType != LogicNegation)//������ i64
					return false;
			}
			v1->setData(opdat);
			return true;
		}
		void set(nodePtr v1) {}
		void copy(nodePtr v1, nodePtr v2) {}

	}
}