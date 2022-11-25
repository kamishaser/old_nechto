#pragma once
#include "node.h"
#include "nodeTypeProperties.h"

#include "text.h"
#include "externalObject.h"
#include "connectionIterator.h"
#include <functional>
#include <map>
#include <set>

namespace nechto
{
	//���� ������� �� 2 ������:
	//����, (���� � �����)
	//�������������� ������ (��� � ��� ���������, ������������ ��� ������)
	
	enum class serialezeStage : char
	{
		nodeBegin,
		mainChain,
		
		nodeEnd,

	};
	class serializer
	{
		//�������� �������� ������������ ����� ����������� � ���������� ���� ������
		//����� ���� ��� ������ �������� ��������� �� ���������
		//��� ��� ���� ����������� ��������������� ��� ������������ �������
		//���������� ����������� ������ � ��� ������, ���� ��� ���� ���������
		//�� ���� �� ������ ����������� ����
	private:
		using writeFunctionType = std::function<void(const char*, uint32_t)>;
		using sStage = serialezeStage;
		writeFunctionType write = nullptr;

		//���� ����������� � ���� ��������, ������� �������� �� ������ ����������
		//�������������� � ����� ��������� �� ���
		//������ ��� �������� �� ������ ����� ������, �� ������� � ���� ������������
	public:
		serializer(writeFunctionType sf)
			:write(sf) {}

		void serialize(nodePtr v1)
		{
			serialezeStage stage = serialezeStage::nodeBegin;
			writeByPointer(&stage);
			writeByValue(v1);//������ ������
			writeTypeAndSubtype(v1);
			writeChain(v1);
			switch (v1->getType())
			{
			case node::Variable:
				writeByValue(v1->getData<i64>());
				break;
			case node::Text:
				writeu16string(text::get(v1));
				break;
			case node::Group:
				writeGroup(v1);
			default:
				break;
			}
		}
		void end()
		{
			writeByValue(sStage::nodeEnd);
		}
	private:
		void writeTypeAndSubtype(nodePtr v1)
		{
			char type = v1->getType();
			char subtype = v1->getSubtype();
			writeByPointer(&type);
			writeByPointer(&subtype);
		}
		//���������� ��� ������������ ���������� � ���� ��� ����
		void writeNumConnections(hubIterator hi)
		{
			for (int i = 0; i < 4; ++i)
			{
				hi.setLocalPos(i);
				writeByValue<nodePtr>(hi.get());
			}
		}
		void writeu16string(const std::u16string& string)
		{
			int32_t size = string.size();
			writeByPointer(&size);
			for (int32_t i = 0; i < size; ++i)
				write(reinterpret_cast<const char*>(&string[i]), sizeof(wchar_t));
		}
		void writeGroup(nodePtr group)
		{
			assert(typeCompare(group, node::Group));
			groupIterator gi(group);
			writeNumConnections(gi);//���� ��� ���� ������
			while (gi.GoToNextHub()) 
			{
				writeByValue(true);
				writeNumConnections(gi);				
			}
			writeByValue(false);
		}
		void writeChain(nodePtr v1)
		{
			connectionIterator ci(v1);
			writeNumConnections(ci);//���� ��� ���� ������
			while (ci.GoToNextHub())
			{
				writeByValue(true);
				writeNumConnections(ci);
			}
			writeByValue(false);
		}
		//����������� ���������� ���� ������
		template<class TCon>
		void writeByPointer(const TCon* data)
		{
			write(reinterpret_cast<const char*>(data), sizeof(TCon));
		}
		template <class TCon>
		void writeByValue(const TCon& ref)
		{
			write(reinterpret_cast<const char*>(&ref), sizeof(ref));
		}
	};
	//������ (��������) nechto
	class deserializer
	{
		using readFunctionType = std::function<void(char*, uint32_t)>;
		using sStage = serialezeStage;
		/*map �������� ������� ���������� ��� ������ ������������� ����
		first - ������ ����� ���������� (������ ���������� ����)
		second->first - ������ ����� ���������� (��� ����������� ����)
		second->second - ����� ������� ����������*/
		using conMapType = std::multimap<nodePtr, hubIterator>;

		//������ �� ������������ ��� ������ ��� �� ����������� ����
		struct noname//�� ���� ��������� ��� �����
		{
			conMapType conMap;
			/*
			����� ����������, ����������� �� ��� ����.
			���� ������ ��������� � ������ �������� ��� & �������
			���� �������� ������ - ����� - ������������� �����
			*/
			std::vector<std::pair<nodePtr, i64>> iteratorSet;
		};
		std::map<nodePtr, noname> unloadedConnections;
		readFunctionType  read = nullptr;
		
	public:
		deserializer(readFunctionType lf)
			:read(lf) {}
		nodePtr deserialize()
		{
			serialezeStage stage = readStage();
			if (stage != sStage::nodeBegin)
				return nullNodePtr;
			const nodePtr old = readByValue<nodePtr>();
			nodePtr v1 = readTypeSubtypeAndCreate();
			conMapType& conMap = unloadedConnections.emplace(old, noname()).first->second.conMap;
			readChain(conMap, v1, old);
			switch (v1->getType())
			{
			case node::Variable:
				v1->setData<i64>(readByValue<i64>()) ;
				break;
			case node::Text:
				text::set(v1, readu16string());
				break;
			case node::Group:
				readGroup(conMap, v1, old);
			default:
				break;
			}
			return v1;
		}
	private:
		//������� ������ ������������
		serialezeStage readStage()
		{
			serialezeStage temp;
			read(reinterpret_cast<char*>(&temp), sizeof(temp));
			return temp;
		}
		nodePtr readTypeSubtypeAndCreate()
		{
			char type;
			char subtype;
			read(&type, 1);
			read(&subtype, 1);
			assert(type != node::Deleted);
			return newNode(type, subtype);
		}
		/*������� ������������ ���������� ���� ��� ����*/
		void readNumConnections(conMapType& conMap, hubIterator hi, nodePtr v1old)
		{
			for (int i = 0; i < 4; ++i)
			{
				hi.setLocalPos(i);
				nodePtr old = readAddress();//������ �����
				if (old.exist())
				{
					conMapType::iterator mapI = conMap.find(old);
					if (mapI != conMap.end())//���� �������� ���������� ��� ���������
					{//��� ������ ���� �����-�� ��������
						IterIterConnect(hi, mapI->second);//������������ �����������
						conMap.erase(mapI);
					}
					else
					{
						auto mapI2 = unloadedConnections.find(old);
						//���� ���� ����� �� ���� �� ����������
						if (mapI2 == unloadedConnections.end())
							mapI2 = unloadedConnections.emplace(old, noname()).first;
						mapI2->second.conMap.emplace(v1old, hi);
					}
				}
			}
		}
		nodePtr readAddress()
		{
			nodePtr temp;
			read(reinterpret_cast<char*>(&temp), sizeof(temp));
			return temp;
		}
		std::u16string readu16string()
		{
			int32_t size;
			read(reinterpret_cast<char*>(&size), sizeof(size));
			std::u16string string;
			string.resize(size);
			for (int32_t i = 0; i < size; ++i)
				read(reinterpret_cast<char*>(&string[i]), sizeof(wchar_t));
			return string;
		}
		

		void readGroup(conMapType& conMap, nodePtr group, nodePtr groupOld)
		{
			assert(typeCompare(group, node::Group));
			groupIterator gi(group);
			
			readNumConnections(conMap, gi, groupOld);//���� ��� ���� ������
			while (readByValue<bool>())
			{
				hub::insert(gi.currentHub, group);
				gi.currentHub = hub::next(gi.currentHub);
				readNumConnections(conMap, gi, groupOld);
			}
		}
		void readChain(conMapType& conMap, nodePtr v1, nodePtr v1Old)
		{
			connectionIterator ci(v1);
			readNumConnections(conMap, ci, v1Old);//���� ��� ���� ������
			while (readByValue<bool>())
			{
				hub::insertBack(ci.currentHub, v1);
				ci.currentHub = hub::next(ci.currentHub);
				readNumConnections(conMap, ci, v1Old);
			}
		}


		template <class TCon>
		TCon readByValue()
		{
			TCon temp;
			read(reinterpret_cast<char*>(&temp), sizeof(temp));
			return temp;
		}
	};
}
