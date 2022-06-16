#pragma once
#include "node.h"
#include <filesystem>
#include <fstream>
#include <map>
#include <set>

namespace nechto
{
	class nechtoOutFileStream
	{//поток файлового сохранения нечто

		//основная проблема сохранения нечто заключается в сохранении всех связей
		//адрес ноды при разных запусках программы не совпадает
		//так как ноды сохраняются последовательно без определённого порядка
		//соединения сохраняются только в том случае, если обе ноды сохранены
		//то есть на второй сохраняемой ноде
	private:
		std::set<nodePtr> savedNodes;//список сохранённых нод
		std::ofstream out;//поток файлового сохранения

		char NumConnectionNumber(nodePtr v1, nodePtr v2)
		{//если соединение нумерованное возрвщает номер, иначе -1
			for (char i = 0; i < 4; i++)
				if (v1->connection[i].load() == v2)
					return i;
			return -1;
		}
		template<class T>
		void write(T* data)
		{
			out.write(reinterpret_cast<char*>(data), sizeof(T));
		}

	public:
		bool start(std::filesystem::path path)
		{//открытие файла
			out.open(path, std::ios::binary | std::ios::trunc | std::ios::out);
			return out.is_open();
		}
		bool isOpen()
		{//проверка
			return out.is_open();
		}
		void end()
		{
			savedNodes.clear();
			out.close();
		}
		void clear()
		{//полная отчистка списка сохранённых нод
			savedNodes.clear();
			out.clear();
		}
		bool isSaved(nodePtr v1)
		{//проверка сохранена ли нода
			return savedNodes.contains(v1);
		}
		nechtoOutFileStream& operator<<(const nodePtr v1)
		{//сохранение ноды
			savedNodes.insert(v1);//добавление в список сохранённых
			
			//1)сохранение данных ноды
			auto type    = v1->type.load();
			auto subtype = v1->subtype.load();
			auto data    = v1->data.load();
			write(&v1);
			write(&type);
			write(&subtype);
			write(&data);
			//2)сохранение нумерованных соединений
			for (int i = 0; i < 4; i++)
			{
				nodePtr temp;
				char backNumber;
				//так как все соединения двустороннии важно знать номер обратного соединения
				if (savedNodes.contains(v1->connection[i].load()))
				{
					temp = v1->connection[i].load();
					backNumber = NumConnectionNumber(temp, v1);
				}
				else
				{
					temp = nullNodePtr;
					backNumber = -1;
				}
				//сохранение производится даже если нет соединения
				write(&temp);
				write(&backNumber);
			}
			//3)сохранение соединений по хабам
			nodePtr hubIterator = v1;
			while(hubIterator->hasHub())
			{
				hubIterator = hubIterator->hubConnection;
				for(int i = 0; i < 4; i++)
					if (hubIterator->hasConnection(i))
					{
						nodePtr temp = hubIterator->connection[i].load();
						if (savedNodes.contains(temp))
						{
							char backNumber = NumConnectionNumber(temp, v1);
							write(&temp);
							write(&backNumber);
						}
					}
			}
			out.write(reinterpret_cast<const char*>(&nullNodePtr), sizeof(nullNodePtr));
			return *this;
		}
		
	};

	bool nechtoFileLoad(std::filesystem::path path, std::function<void(nodePtr)> nodeLoad)
	{
		std::ifstream in;
		in.open(path, std::ios::binary);
		if (!in.is_open())
			return false;

		return true;
	}
	class nechtoInFileStream
	{
		std::map<nodePtr, nodePtr> loadedNodes;//map загруженных нод
		//ноды сохраняются с теми адресами, которые занимали на момент сохранения
		//соответственно и связи прописаны по ним
		//однако при загрузке им выдают новые адреса, по которым и надо коннектиться
		

	public:

		
	};
}
