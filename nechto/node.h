#pragma once

#include "staticAllocator.h"

#include <memory>
#include <vector>
#include <atomic>
#include <mutex>
#include <thread>
#include <cassert>
#include <iostream>
#include <string>
#include <functional>
#include <compare>

class commandLine;
namespace nechto
{
	namespace nodeStorage
	{
		class Terminal;
	}
	
	using ushort = unsigned short;
	using i64 = int64_t;
	using f64 = double;

	struct node //���� ���� nechto
 	{
		class ptr
		{
			ushort first;
			ushort second;

			ptr(ushort f, ushort s){
				first = f;
				second = s;
			}
			friend class nodeStorage::Terminal;
			friend class commandLine;
		public:
			ushort getFirst() const
			{
				return first;
			}
			ushort getSecond() const
			{
				return second;
			}
			ptr()
			{
				first = 0;
				second = 0;
			}
			bool exist() const
			{
				return first != 0;
			}
			operator bool() const
			{
				return exist();
			}
			bool check() const;
			
			node* operator-> ();
			node* operator* ();

			const node* operator-> () const;
			const node* operator* () const;

			/*bool operator<(const ptr& v2)const
			{
				const void* v1p = this;
				const void* v2p = &v2;
				return *reinterpret_cast<const uint32_t*>(v1p) < *reinterpret_cast<const uint32_t*>(v2p);
			}
			bool operator==(const ptr& v2)const
			{
				return (first == v2.first) && (second == v2.second);
			}
			bool operator!=(const ptr& v2)const
			{
				return (first != v2.first) || (second != v2.second);
			}*/
			auto operator<=>(const ptr&) const = default;
		};
	private:
		std::atomic<char> type;//��� ����
		std::atomic<char> subtype;//������ ����
		std::atomic<bool> correctness�hecked = false;
	public:
		std::atomic<i64> data = 0;//������ ����
		std::atomic<ptr> connection[4];
		std::atomic<ptr> hubConnection;
	
		static_assert(std::atomic<i64>::is_always_lock_free);
		friend void setTypeAndSubtype(ptr, char, char);
		

		template <class TCon>
		const TCon getData() const //��������� ������ � ������� <TCon>
		{
			assert(sizeof(TCon) <= sizeof(i64));
			i64 temp = data.load();
			return *static_cast<TCon*>(static_cast<void*>(&temp));
		}
		template <class TCon>
		void setData(TCon Data) //������ ������ � ������� TCon
		{
			assert(sizeof(TCon) <= sizeof(i64));
			i64 temp = *static_cast<i64*>(static_cast<void*>(&Data));
			data.store(temp);
		}

		bool hasConnection(int number) const //�������� ������� ���������� �� ������	
		{
			assert(number < 4);
			return (connection[number].load());
		}
		bool hasHub() const //�������� ������� ���������� �� ������	
		{
			return (hubConnection.load());
		}
		void correctnessHasNotBeenChecked()
		{
			correctness�hecked = false;
		}
		int connectionType(int number) const //��������� ���� ���� ������������ �� ������
		{
			if (!hasConnection(number)) return 0;
			return connection[number].load()->getType();
		}
		int connectionSubtype(int number) const//��������� ������� ���� ������������ �� ������
		{
			if (!hasConnection(number)) return 0;
			return connection[number].load()->getSubtype();
		}
		char getType() const
		{
			return type;
		}
		char getSubtype() const 
		{
			return subtype;
		}
		enum Type //������ ����� ���
		{
			Deleted,
			Hub,					//������������
			Variable,				//������-���������� �������� ����, ���������� ������ ��������� (�������� ��� ���� ������������)
			MathOperator,			//�������������� ��������
			ConditionalBranching,	//if
			ExternalFunction,		//�������, �� ���������� ������ nechto
			Tag,					//�����
			Pointer,				//��������� �� ������
			Array
		};
	};
	namespace variable
	{
		enum Type
		{
			F64 = 0,//false
			I64 = 1 //true
		};
	}
	namespace mathOperator
	{
		enum Type
		{
			Assigment,		// =
			UnaryMinus,		// 0-

			Addition,		// +
			Subtraction,	// -

			Multiplication, // *
			Division,		// /

			Equal,			// ==
			NotEqual,		// !=

			Less,			// <
			Greater,		// >
			LessOrEqual,	// <=
			GreaterOrEqual,	// >=

			LogicNegation,	// !
			LogicAnd,		// &&
			LogicOr,		// ||

			Increment,		// ++
			Decrement,		// --
		};
	}
	namespace tag
	{

		enum Type
		{
			Comment, //����������� ��������� �����������
			Name,

			ExternalConnection, //������� �����������. ������ ���� ����� �������������� �����.
			//���� ������� ������� ����������� ������ �������.
			//��������� ���������, �� ������� �������� ����������� ��������� ��������� � 
			//����� ���� ������� ��������� ������, (����� �� ����� ����������)
			Attribute
		};
	}
	namespace pointer
	{
		enum Type
		{
			Reference = 0,//��������� ������
			ConIter,//�������� ����������
			ArrayIter//�������� �������
		};
	}	

	using nodePtr = node::ptr;
	using nodeEvent = std::function<void(nodePtr)>;
	using nodeConnectionEvent = std::function<void(nodePtr, nodePtr)>;
	const nodePtr nullNodePtr = nodePtr(); //������ nullptr

	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////
	//��, ��� ���� ���� ������ ���� �� �����������

	namespace nodeStorage
	{
		const int maxNumOfAllocators = 16; //������������ ���������� ���������� �����������
		static std::unique_ptr<staticAllocator<node>> content[maxNumOfAllocators]; //������ �����������
		static ushort occupancy[maxNumOfAllocators]; //������ ���������
		static std::atomic<ushort> sflag; //����� ����������� ����������
		static std::atomic<ushort> freeSpace; //���������� ��������� �����������

		static std::mutex changeAllocatorBlock;



		static void toFreeAllocator(ushort number)//������������ ����������
		{
			changeAllocatorBlock.lock();
			occupancy[sflag.fetch_add(-1)] = number;
			++freeSpace;
			changeAllocatorBlock.unlock();
		}
		static ushort getFreeAllocator()//������ ����������
		{
			changeAllocatorBlock.lock();
			--freeSpace;
			ushort number;
			while (true)
			{
				number = sflag.fetch_add(1);
				if (!content[occupancy[number]])
				{
					changeAllocatorBlock.unlock();
					content[occupancy[number]] = std::make_unique<staticAllocator<node>>();
					return number;
				}
				if (content[occupancy[number]]->freeSpace() > 256)
				{
					changeAllocatorBlock.unlock();
					return number;
				}
			}

		}

		staticAllocator<node>* getAllocator(const ushort number)
		{
			assert((number > 0) && (number < maxNumOfAllocators));
			assert(content[number]);
			return content[number].get();
		}

		static ushort getFreeSpace()
		{
			return freeSpace;
		}
		static void reset()
		{//������ ����� ���������. ������� ��������!
			changeAllocatorBlock.lock();
			for (int i = 0; i < maxNumOfAllocators; i++)
				occupancy[i] = i;
			sflag = 1;
			freeSpace = maxNumOfAllocators - 1;
			changeAllocatorBlock.unlock();
		}

		//////////////////////////////////////////////////////////////////////////////////////////

		class Terminal
		{//����������� ��������� ������� ��� ���������� ������ ������������ � �����������
			std::vector<ushort> localAllocatorSet;//������ ������� ����������������� �����������
			ushort currentAllocatorNumber;//����� �������� ����������
			staticAllocator<node>* currentAllocator;//������� ���������

			//����� ������������� � ������ ������ ����������
			void changeCurrentAllocator()
			{
				for (auto i = localAllocatorSet.begin(); i != localAllocatorSet.end(); ++i)
				{
					if (getAllocator(*i)->freeSpace() > 256)
					{
						currentAllocatorNumber = *i;

						currentAllocator = getAllocator(*i);
						return;
					}
				}

				currentAllocatorNumber = getFreeAllocator();
				currentAllocator = getAllocator(currentAllocatorNumber);
				localAllocatorSet.push_back(currentAllocatorNumber);
			}//������� ������� ���������
			static bool isFistTerminal;//������ �������� ������������� ���������
		public:

			explicit Terminal()
			{
				if (isFistTerminal)
				{
					isFistTerminal = false;
					reset();
				}
				changeCurrentAllocator();
			}
			~Terminal()
			{
				for (auto i = localAllocatorSet.begin(); i != localAllocatorSet.end(); ++i)
				{
					toFreeAllocator(*i);//������������ ������������ ����������, ����� �� ���������� ��� ������ ����������
				}
			}
			const nodePtr allocate()
			{
				assert(currentAllocator != nullptr);
				if (currentAllocator->freeSpace() <= static_cast<ushort>(256))
					changeCurrentAllocator();
				nodePtr id;
				id.first = currentAllocatorNumber;
				id.second = currentAllocator->allocate();
				return id;
			}
			void deallocate(const nodePtr id)
			{
				assert(getAllocator(id.first) != nullptr);
				getAllocator(id.first)->deallocate(id.second);
			}
		};
		bool Terminal::isFistTerminal = true;
		thread_local Terminal terminal;

	}
	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////

	node* node::ptr::operator-> ()
	{
		assert(exist());
		return nodeStorage::getAllocator(first)->get(second);
	}
	node* node::ptr::operator* ()
	{
		assert(exist());
		return nodeStorage::getAllocator(first)->get(second);
	}

	const node* node::ptr::operator-> () const
	{
		assert(exist());
		return nodeStorage::getAllocator(first)->get(second);
	}
	const node* node::ptr::operator* () const
	{
		assert(exist());
		return nodeStorage::getAllocator(first)->get(second);
	}
}