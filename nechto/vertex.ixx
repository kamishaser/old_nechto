export module vertex;
import allocatedStorage;

import <atomic>;
import <utility>;
import <cassert>;

export namespace nechto
{
	using ushort = unsigned short;
	struct vertex
 	{
		struct ptr
		{
			ushort first;
			ushort second;

			ptr(ushort f = 0, ushort s = 0){
				first = f;
				second = s;
			}
			ptr(std::pair<ushort, ushort> address)
			{
				first = address.first;
				second = address.second;
			}
			vertex* operator-> () const
			{
				return allocatedStorage<vertex>::getAllocator(first)->get(second);
			}
			vertex* operator* () const
			{
				return allocatedStorage<vertex>::getAllocator(first)->get(second);
			}
			bool exist() const
			{
				return first != 0;
			}
			operator bool() const 
			{ 
				return exist(); 
			}

			operator const std::pair<ushort, ushort>() const
			{
				return std::pair<ushort, ushort>(first, second);
			}
		};
		
		std::atomic<size_t> data = 0;
		std::atomic<std::pair<ushort, ushort>> type;
		std::atomic<ptr> connection[4];
		std::atomic<ptr> hubConnection;

		template <class TCon>
		const TCon getData() const
		{
			assert(sizeof(TCon) <= sizeof(size_t));
			size_t temp = data.load();
			return *static_cast<TCon*>(static_cast<void*>(&temp));
		}
		template <class TCon>
		void setData(TCon Data)
		{
			assert(sizeof(TCon) <= sizeof(size_t));
			size_t temp = *static_cast<size_t*>(static_cast<void*>(&Data));
			data.store(temp);
		}

		enum vType
		{
			Empty,					//пустой объект
			Hub,					//разветвитель
			Tag,					//помечнный извне объект (односторонн€€ св€зь может быть только к метке 
									//(о метке знает только один объект))
			Variable,				//объект-переменна€ базового типа, хнан€ща€с€ внутри алгоритма (одинаков дл€ всех исполнителей)
			Pointer,				//указатель на объект
			Assignment,				//оператор присваивани€ (отличаетс€ от обычного математического)
			Function,				//функци€, не €вл€юща€с€ частью nechto
			MathOperator,			//математический оператор
			ConditionalBranching,	//if
			BranchingMerge,			//сли€ние ветвей
			Segment,				//определЄнный отрезок алгоритма, вход и выход из которого осуществл€етс€ только в одном месте
			TagCall,				//вызов алгоритма по тегу
			NumberOfTypes			//не объект. „исло, обозначающее количество типов
		};
	};

	using vertexPtr = vertex::ptr;

	const vertexPtr nullVertexPtr(0, 0);
}