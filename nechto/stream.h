#pragma once
#include "node.h"
#include "nodeTypeProperties.h"

#include "textOut.h"
#include <functional>
#include <map>
#include <set>

namespace nechto
{
	//���� ������� �� 2 ������:
	//����, (���� � �����)
	//�������������� ������ (��� � ��� ���������, ������������ ��� ������)

	/*
	* ������� ������� ������ ��������� ���������� � json �����
	"nodeList":
	[
		"node":
		{
			"address": ... ...
			"type":    ...
			"subtype": ...
			"data":    ...

			"additionalDataSize": ... //uint32_t
			//�������������� ������ ����. ��� ��� �������� ��� �������� �� �����
			"addeteionalData": "...",

			"numberedConnections":
			[
				"con0":
				{
				"address":  ... ...
				"backNumber":   ... 
				},
				"con1": ...  
				"con2": ...
				"con3": ...
			],			
			"hubConnections":
			//���������� ������ � ���� ���������, ������� ��� ���������
			[
				"hubcon":
				{
				"address":  ... ...
				"backNumber":   ... 
				},
				"hubcon":       ...
				...
				nullNodePtr //�����������
			]
		}
		...
	]
	}
	*/



	class stream
	{//����� ��������� ���������� �����

		//��� ����� �������� ��� �� ����������, ��� � �� ��������, 
		// ��������� solid,
		// �� � �� ���� ������� �������� ��� ����������
		//


		//�������� �������� ���������� ����� ����������� � ���������� ���� ������
		//����� ���� ��� ������ �������� ��������� �� ���������
		//��� ��� ���� ����������� ��������������� ��� ������������ �������
		//���������� ����������� ������ � ��� ������, ���� ��� ���� ���������
		//�� ���� �� ������ ����������� ����
	private:
		const char hubBack = -1;

		std::set<nodePtr> savedNodes;//������ ����������� ���

		
		//std::ofstream out;//����� ��������� ����������
		
		
		char NumConnectionNumber(nodePtr v1, nodePtr v2)
		{//���� ���������� ������������ ��������� �����, ����� -1
			for (char i = 0; i < 4; i++)
				if (v1->connection[i].load() == v2)
					return i;
			return hubBack;
		}
		/*template<class T>
		void write(const T* data)
		{
			out.write(reinterpret_cast<const char*>(data), sizeof(T));
		}*/

		template<class T>
		void writeElement(const T* data)
		{
			write(reinterpret_cast<const char*>(data), sizeof(T));
		}
		nodePtr readAddress()
		{
			nodePtr temp;
			read(reinterpret_cast<char*>(&temp), sizeof(temp));
			return temp;
		}
		char readBackNumber()
		{
			char backNumber;
			read(reinterpret_cast<char*>(&backNumber), sizeof(backNumber));
			return backNumber;
		}

	public:
		std::function<std::string(nodePtr)>			getAdditionalNodeData = nullptr; 
		std::function<void(nodePtr, std::string)>	setAdditionalNodeData = nullptr;
		//�������������� ������ ����

		std::function<void(const char*, uint32_t size)> write = nullptr;
		std::function<void(char*,		uint32_t size)> read  = nullptr;

		void clear()
		{//������ �������� ������ ����������� ���
			savedNodes.clear();
		}
		bool isSaved(nodePtr v1)
		{//�������� ��������� �� ����
			return savedNodes.contains(v1);
		}
		void saveEnd()
		{
			writeElement(&nullNodePtr);
			savedNodes.clear();
		}
		void save(const nodePtr v1)
		{//���������� ����
			assert(v1->type != node::Hub);
			if (isSaved(v1))
				return;
			savedNodes.insert(v1);//���������� � ������ �����������
			
			//1)���������� ������ ����
			auto type    = v1->type.load();
			auto subtype = v1->subtype.load();
			auto data    = v1->data.load();
			writeElement(&v1);		//�����
			writeElement(&type);
			if(hasSubType(v1))
				writeElement(&subtype);
			if(hasStaticData(v1))
				writeElement(&data);
			if (hasStaticAdData(v1))
			{
				std::string adData;
				if (getAdditionalNodeData != nullptr)
					adData = getAdditionalNodeData(v1);
				const uint32_t adDataSize = static_cast<uint32_t>(adData.size());
				writeElement(&adDataSize);
				for (uint32_t i = 0; i < adDataSize; i++)
				{
					char temp = adData[i];
					writeElement(&temp);
				}
			}

			//2)���������� ������������ ����������
			for (int i = 0; i < 4; i++)
			{
				nodePtr temp;
				char backNumber;
				//��� ��� ��� ���������� ������������ ����� ����� ����� ��������� ����������
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
				//���������� ������������ ���� ���� ��� ����������
				writeElement(&temp);
				writeElement(&backNumber);
			}
			//3)���������� ���������� �� �����
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
							writeElement(&temp);
							writeElement(&backNumber);
						}
					}
			}
			writeElement(&nullNodePtr);
			return;
		}
		void load()
		{
			std::map<nodePtr, nodePtr> loadedNodes;//map ����������� ���
			//���� ����������� � ���� ��������, ������� �������� �� ������ ����������
			//�������������� � ����� ��������� �� ���
			//������ ��� �������� �� ������ ����� ������, �� ������� � ���� ������������
			while (true)
			{
				
				ushort typeBuffer, subtypeBuffer;
				size_t dataBuffer;
				nodePtr oldAddress;
				oldAddress = readAddress();
				if (!oldAddress.exist())
					break;
				nodePtr vload = newNode();
				loadedNodes.emplace(oldAddress, vload);
				read(reinterpret_cast<char*>(&typeBuffer), sizeof(typeBuffer));
				vload->type = typeBuffer;
				if (hasSubType(vload))
				{
					read(reinterpret_cast<char*>(&subtypeBuffer), sizeof(subtypeBuffer));
					vload->subtype = subtypeBuffer;
				}
				if (hasStaticData(vload))
				{
					read(reinterpret_cast<char*>(&dataBuffer), sizeof(dataBuffer));
					vload->data = dataBuffer;
				}
				std::string adData;
				if (hasStaticAdData(vload))
				{
					uint32_t adDataSize;
					read(reinterpret_cast<char*>(&adDataSize), sizeof(adDataSize));

					if (adDataSize != 0)
					{
						adData.resize(adDataSize);
						for (uint32_t i = 0; i < adDataSize; i++)
						{
							char temp;
							read(&temp, 1);
							adData[i] = temp;
						}
					}
				}
				if(setAdditionalNodeData != nullptr)
					setAdditionalNodeData(vload, adData);
				for (int i = 0; i < 4; i++)
				{
					nodePtr conAddress = readAddress();
					char backNumber = readBackNumber();
					
					if (conAddress.exist())
					{
						assert(loadedNodes.contains(conAddress));
						conAddress = loadedNodes.at(conAddress);
						
						if (conAddress.exist())
							if (backNumber == hubBack)
								NumHubConnect(vload, conAddress, i);
							else
								NumNumConnect(vload, conAddress, i, backNumber);
					}

				}
				while (true)
				{
					nodePtr conAddress = readAddress();
					std::cout << to_string(conAddress) << std::endl;
					if (!conAddress.exist())
						break;
					assert(loadedNodes.contains(conAddress));
					conAddress = loadedNodes.at(conAddress);
					char backNumber = readBackNumber();
					if (backNumber != hubBack)
						NumHubConnect(conAddress, vload, backNumber);
					else
						HubHubConnect(conAddress, vload);
				}
			}
		}
	};
}