export module staticAllocator;
import <atomic>;

export namespace nechto
{
	using ushort = unsigned short;
	template <class TCon>
	class staticAllocator
	{
	private:
		TCon content[65536]; //массив элементов
		ushort occupancy[65536]; //массив занятости
		std::atomic<ushort> gflag = 0;
		//номер занимаемого элемента
		std::atomic<ushort> sflag = 0;
		//номер освобождаемого элемента
		std::atomic<ushort> freespace = 65535; //свободное место

	public:

		staticAllocator()
		{//заполнение списка занятости
			for (int i = 0; i <= 65535; i++)
				occupancy[i] = i;
		}
		TCon* get(const ushort number)
		{
			return &content[number];
		}
		TCon* operator[](const ushort number)
		{
			return &content[number];
		}
		

		void deallocate(ushort number) //освобождение элемента
		{
			++freespace;
			occupancy[sflag.fetch_add(1)] = number;
		}
		ushort allocate() //взятие элемента
		{
			--freespace;
			return occupancy[gflag.fetch_add(1)];
		}
		ushort freeSpace() //количество элементов в очереди
		{
			return freespace.load();
		}
		void release()
		{//полный сброс хранилища. Опасная операция!
			gflag = 0;
			sflag = 0;
			freespace = 65535;
			for (int i = 0; i <= 65535; i++)
				occupancy[i] = i;
		}
	};
}