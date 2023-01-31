#pragma once
#include "nodePtr.h"

#include "text.h"
#include "entity.h"
#include "connectionPointer.h"
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
	static_assert(sizeof(wchar_t) == 2);
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
		std::vector<char> buffer;

		//���� ����������� � ���� ��������, ������� �������� �� ������ ����������
		//�������������� � ����� ��������� �� ���
		//������ ��� �������� �� ������ ����� ������, �� ������� � ���� ������������
	public:
		serializer(writeFunctionType sf)
			:write(sf) {}

		void serialize(nodePtr v1)
		{
			if (!v1.exist())
				return;
			serialezeStage stage = serialezeStage::nodeBegin;
			writeByPointer(&stage);
			writeByValue(v1);//������ ������
			writeTypeAndSubtype(v1);
			writeChain(v1);
			switch (v1.type())
			{
			case nodeT::Variable:
			case nodeT::Vector:
				writeByValue(v1.node()->data);
				break;
			case nodeT::Text:
				writeWstring(*textPtr(v1).getPtr());
				break;
			case nodeT::Group:
				writeGroup(v1);
				break;
			case nodeT::Entity:
				assert(false);
				//writeEntity(v1);
				break;
			case nodeT::Operator:
			case nodeT::Struct:
				assert(false);
				break;
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
			char type = v1.type();
			char subtype = v1.subtype();
			writeByPointer(&type);
			writeByPointer(&subtype);
		}
		//���������� ��� ������������ ���������� � ���� ��� ����
		void writeNumConnections(pointer hi)
		{
			for (int i = 0; i < 4; ++i)
			{
				hi.setLocalPos(i);
				writeByValue<nodePtr>(hi.get());
			}
		}
		void writeWstring(const std::wstring& string)
		{
			int32_t size = string.size();
			writeByPointer(&size);
			for (int32_t i = 0; i < size; ++i)
				write(reinterpret_cast<const char*>(&string[i]), sizeof(wchar_t));
		}
		void writeGroup(groupPtr group)
		{
			assert(typeCompare(group, nodeT::Group));
			groupPointer gi(group);
			writeNumConnections(gi);//���� ��� ���� ������
			while (gi.goToNextHub()) 
			{
				writeByValue(true);
				writeNumConnections(gi);				
			}
			writeByValue(false);
		}
		/*void writeEntity(nonTypedEntityPtr obj)
		{
			if (obj.dataExist())
			{
				writeWstring(obj.getEntityPtr()->getTypeName());
				buffer.clear();
				obj.getEntityPtr()->serialize(buffer, obj);
				writeByValue<ui32>(buffer.size());
				write(buffer.data(), buffer.size());
			}
			else
			{
				writeWstring(std::wstring());
				writeByValue<ui32>(0);
			}
		}*/
		void writeChain(nodePtr v1)
		{
			portPointer ci(v1);
			writeNumConnections(ci);//���� ��� ���� ������
			while (ci.goToNextHub())
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
	//class deserializedEntity :public entity
	//{
	//public:
	//	//������ typeDefinition � �������� �� �� ���� ����������� �������, ��� ���� ������
	//	std::wstring name;
	//	std::vector<char> buffer;
	//	deserializedEntity(const std::wstring& n, ui32 size)
	//		:name(n), buffer(size) {}

	//	virtual void serialize(std::vector<char>& buffer, existing<nodePtr> obj) const override
	//	{
	//		buffer = entityPtr<deserializedEntity>(obj)->buffer;
	//	}
	//	virtual const std::wstring& getTypeName() const override
	//	{
	//		return name;
	//	}
	//};
	//������ (��������) nechto
	class deserializer
	{
		using readFunctionType = std::function<void(char*, uint32_t)>;
		using sStage = serialezeStage;
		/*map �������� ������� ���������� ��� ������ ������������� ����
		first - ������ ����� ���������� (������ ���������� ����)
		second->first - ������ ����� ���������� (��� ����������� ����)
		second->second - ����� ������� ����������*/
		using conMapType = std::multimap<nodePtr, pointer>;

		//������ �� ������������ ��� ������ ��� �� ����������� ����
		struct noname//�� ���� ��������� ��� �����
		{
			conMapType conMap;
			/*
			����� ����������, ����������� �� ��� ����.
			���� ������ ��������� � ������ �������� ��� & �������
			���� �������� ������ - ����� - ������������� �����
			*/
			std::vector<std::pair<nodePtr, i64>> pointerSet;
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
				return nullptr;
			const nodePtr old = readByValue<nodePtr>();
			nodePtr v1 = readTypeSubtypeAndCreate();
			conMapType& conMap = unloadedConnections.emplace(old, noname()).first->second.conMap;
			readChain(conMap, v1, old);
			switch (v1.type())
			{
			case nodeT::Variable:
			case nodeT::Vector:
				v1.setData<i64>(readByValue<i64>());
				break;
			case nodeT::Text:
				textPtr(v1).set(readWstring());
				break;
			case nodeT::Group:
				readGroup(conMap, v1, old);
				break;
			case nodeT::Entity:
				//readEntity(nonTypedEntityPtr(v1));
				assert(false);
				break;
			case nodeT::Operator:
			case nodeT::Struct:
				assert(false);
				break;
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
			unsigned char type = readByValue<unsigned char>();
			unsigned char subtype = readByValue<unsigned char>();
			return creator::createNode(type, subtype);
		}
		/*������� ������������ ���������� ���� ��� ����*/
		void readNumConnections(conMapType& conMap, pointer hi, nodePtr v1old)
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
						connecter::connect(hi, mapI->second);//������������ �����������
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
		std::wstring readWstring()
		{
			int32_t size;
			read(reinterpret_cast<char*>(&size), sizeof(size));
			std::wstring string;
			string.resize(size);
			for (int32_t i = 0; i < size; ++i)
				read(reinterpret_cast<char*>(&string[i]), sizeof(wchar_t));
			return string;
		}
		

		void readGroup(conMapType& conMap, nodePtr group, nodePtr groupOld)
		{
			assert(typeCompare(group, nodeT::Group));
			groupPointer gi(group);
			
			readNumConnections(conMap, gi, groupOld);//���� ��� ���� ������
			while (readByValue<bool>())
			{
				hubManager::insertHub(gi, 1);
				gi.goToNextHub();
				readNumConnections(conMap, gi, groupOld);
			}
		}
		/*void readEntity(nonTypedEntityPtr obj)
		{
			std::wstring name = readWstring();
			ui32 dataSize = readByValue<ui32>();
			if (name.empty() && dataSize == 0)
				return;
			assert(!obj.dataExist());
			auto desObj = new deserializedEntity(name, dataSize);
			obj.set(desObj);
			if (dataSize != 0)
				read(desObj->buffer.data(), dataSize);
		}*/
		void readChain(conMapType& conMap, nodePtr v1, nodePtr v1Old)
		{
			portPointer ci(v1);
			readNumConnections(conMap, ci, v1Old);//���� ��� ���� ������
			while (readByValue<bool>())
			{
				hubManager::insertHub(ci, 1);
				ci.goToNextHub();
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
