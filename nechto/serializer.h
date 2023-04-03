#pragma once
#include "nodePtr.h"
#include "creator.h"
#include "connecter.h"
#include "text.h"
#include "entity.h"
#include "connectionPointer.h"
#include <functional>
#include <map>
#include <set>
#include <stack>

namespace nechto
{
	//файл состоит из 2 частей:
	//граф, (ноды и связи)
	//дополнительные данные (как и что сохранять, определяется вне потока)
	
	enum class serialezeStage : char
	{
		nodeBegin,
		mainChain,
		
		nodeEnd,

	};
	static_assert(sizeof(wchar_t) == 2);
	class serializer
	{
		//основная проблема сериализации нечто заключается в сохранении всех связей
		//адрес ноды при разных запусках программы не совпадает
		//так как ноды сохраняются последовательно без определённого порядка
		//соединения сохраняются только в том случае, если обе ноды сохранены
		//то есть на второй сохраняемой ноде
	private:
		using writeFunctionType = std::function<void(const char*, uint32_t)>;
		writeFunctionType write = nullptr;
		bufferSerInterface entitySerInterface;

		//ноды сохраняются с теми адресами, которые занимали на момент сохранения
		//соответственно и связи прописаны по ним
		//однако при загрузке им выдают новые адреса, по которым и надо коннектиться
	public:
		serializer(writeFunctionType sf)
			:write(sf) {}

		void serialize(nodePtr v1)
		{
			if (!v1.exist())
				return;
			writeByValue(v1);//запись адреса
			writeTypeAndSubtype(v1);
			
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
				writeEntity(v1);
				break;
			case nodeT::Pointer:
				writePointer(pointerPtr(v1));
			case nodeT::Operator:
			case nodeT::Struct:
				assert(false);
				break;
			case nodeT::Hub:
				assert(false);
			default:
				break;
			}
			writeChain(v1);
		}
	private:
		void writeTypeAndSubtype(nodePtr v1)
		{
			char type = v1.type();
			char subtype = v1.subtype();
			writeByPointer(&type);
			writeByPointer(&subtype);
		}
		//записывает все нумерованные соединения в ноде или хабе
		void writeNumConnections(nodePtr node)
		{
			nodeData* n1 = node.node();
			writeByValue(n1->reversePosition);
			for (int i = 0; i < 4; ++i)
			{
				writeByValue(n1->port[i]);
				writeByValue(n1->reverseAddress[i]);
			}
		}
		void writeHub(hubPtr hub)
		{
			writeTypeAndSubtype(hub);
			writeNumConnections(hub);
		}
		void writeAddress(nodePtr address)
		{
			writeByValue(address);
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
			writeByValue<ui32>(group.getNumberOfHubs());
			hubPtr hub = group.firstGroupHub();
			for (ui32 i = 0; i < group.getNumberOfHubs(); ++i)
			{
				writeHub(hub);
				hub = hub.hub();
			}
		}
		void writePointer(pointerPtr node)
		{
			writeByValue(node.getHPPair());
		}
		void writeEntity(entityPtr node)
		{
			if (node.entityExist())
			{
				writeWstring(node.getEntityPtr()->getTypeName());
				entitySerInterface.buffer.clear();
				node.getEntityPtr()->serialize(&entitySerInterface, node);
				writeByValue<ui32>(entitySerInterface.buffer.size());
				write(entitySerInterface.buffer.data(), ui32(entitySerInterface.buffer.size()));
			}
			else
			{
				writeWstring(std::wstring());
				writeByValue<ui32>(0);
			}
		}
		void writeChain(nodePtr v1)
		{
			writeNumConnections(v1);//один хаб есть всегда
			while ((v1 = v1.hub()).exist())
			{
				writeHub(v1);
			}
			writeAddress(nullptr);
		}
		//тривиальное сохранение чего угодно
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
	class deserializer
	{
	public:
		using readFunctionType = std::function<void(char*, uint32_t)>;
		using enDeserType = std::function<bool(enDeserInterface*)>;
	private:
		readFunctionType  read = nullptr;
		enDeserType entityDeserCallBack = nullptr;
		bufferDeserInterface enDeser;

		std::map<nodePtr, nodePtr> oldNewIdMap;
		struct nonConnectedPointer
		{
			pointerPtr node;
			hubPosPair oldHpp;

			bool connect(std::map<nodePtr, nodePtr>& map)
			{
				if (!map.contains(oldHpp.hub))
					return false;
				node.setHPPair(hubPosPair(map[oldHpp.hub], oldHpp.getGlobalPos()));
				return true;
			}
		};
		std::stack<nonConnectedPointer> pointers;
		
	public:
		deserializer(readFunctionType lf, enDeserType enDeserCallBack)
			:read(lf), entityDeserCallBack(enDeserCallBack) {}
		nodePtr deserialize()
		{
			const nodePtr old = readAddress();
			char type = readUchar();
			char subtype = readUchar();
			nodePtr node;
			if (!isOperation(type))
			{
				switch (type)
				{
				case nechto::nodeT::Group:
					node = creator::createGroup(subtype, readByValue<ui32>());
					oldNewIdMap.emplace(old, node);
					readGroup(node);
					break;
				case nechto::nodeT::Pointer:
					node = creator::createPointer(subtype);
					oldNewIdMap.emplace(old, node);
					readPointer(pointerPtr(node));
					break;
				case nechto::nodeT::Struct:
					node = creator::createStruct(subtype);
					oldNewIdMap.emplace(old, node);
					//readStruct(node);
					break;
				case nechto::nodeT::Variable:
					node = creator::createVariable(subtype);
					oldNewIdMap.emplace(old, node);
					node.setData(readByValue<i64>());//тут без разницы, i64 или f64
					break;
				case nechto::nodeT::Vector:
					node = creator::createVector(subtype);
					oldNewIdMap.emplace(old, node);
					node.setData(readByValue<i64>());//тут без разницы, i64 или f64 или vector
					break;
				case nechto::nodeT::Entity:
					node = creator::createEntity(subtype);
					oldNewIdMap.emplace(old, node);
					readEntity(node);
					break;
				case nechto::nodeT::Text:
					node = creator::createText(subtype);
					oldNewIdMap.emplace(old, node);
					readText(node);
					break;
				case nechto::nodeT::Operator:
					node = creator::createOperator(subtype);
					oldNewIdMap.emplace(old, node);
					//readOperator(node);
					break;
				default:
					assert(false);
				}
			}
			else
			{
				node = creator::createNode(type, subtype);
				oldNewIdMap.emplace(old, node);
				readOperation(node);
			}
		}
		void end()
		{
			while (!pointers.empty())
			{
				pointers.top().connect(oldNewIdMap);
				pointers.pop();
			}
		}
	private:
		void readOperation(nodePtr node)//тут пока ничего не происходит
		{
		
		}
		uchar readUchar()
		{
			return readByValue<uchar>();
		};
		pointer reversePort(nodePtr revNode, nodePtr revHub, ui32 revLocPos)
		{
			if (revHub == revNode)
				return pointer(revNode, hubPosPair(revHub, revLocPos));
			return pointer(revNode, hubPosPair(revHub,
				hubPtr(revHub).number() * 4 + revLocPos));
		}
		/*считать нумерованные соединения ноды или хаба*/
		void readNumConnections(nodePtr node, nodePtr hub, ui32 hubNumber)
		{
			nodeData* h1 = hub.node();
			h1->reversePosition = readByValue<reverseConnectionPosition>();
			for (int i = 0; i < 4; ++i)
			{
				ui32 globalPos = (hubNumber << 2) + i;
				nodePtr oldConnectionAddress = readAddress();
				nodePtr oldReverseHubAddress = readAddress();
				pointer p1(node, hubPosPair(hub, globalPos));
				if (oldNewIdMap.contains(oldConnectionAddress) &&
					((oldNewIdMap[oldConnectionAddress] != node) ||
						oldNewIdMap.contains(oldReverseHubAddress)))
				{
					nodePtr content = oldNewIdMap[oldConnectionAddress];
					pointer revPort = reversePort(content,
						oldNewIdMap[oldReverseHubAddress], hub.reverseLocalPos(i));
					connecter::connect(p1, revPort);
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
		
		void readPointer(pointerPtr node)
		{
			pointers.emplace(nonConnectedPointer{ node, readByValue<hubPosPair>() });
		}
		void readGroup(groupPtr group)
		{
			hubPtr hub = group.firstGroupHub();
			for (int i = 0; i < group.getNumberOfHubs(); ++i)
			{
				nodePtr old = readAddress();
				oldNewIdMap.emplace(old, hub);
				uchar type = readUchar();
				uchar subtype = readUchar();
				assert(type == nodeT::Hub);
				assert(subtype == 1);
				readNumConnections(group, hub, i);
			}
		}
		void readEntity(entityPtr entity)
		{
			enDeser.typeName = readWstring();
			ui32 dataSize = readByValue<ui32>();
			if (enDeser.typeName.empty() && dataSize == 0)
				return;
			assert(!entity.entityExist());
			enDeser.buffer.resize(dataSize);
			enDeser.enNode = entity;

			read(&enDeser.buffer[0], dataSize);
			entityDeserCallBack(&enDeser);
		}
		void readChain(nodePtr node)
		{
			readNumConnections(node, node, 0);
			nodePtr oldhub = readAddress();
			nodePtr previousHub = node;
			ui32 previousNumber = 0;
			while (oldhub != nullptr)
			{
				uchar type = readUchar();
				uchar subtype = readUchar();
				hubPtr hub = creator::createHub(0);
				oldNewIdMap.emplace(oldhub, hub);
				hub.connect(previousHub, previousNumber);
				++previousNumber;
				readNumConnections(node, hub, previousNumber);
				oldhub = readAddress();
			}
		}
		void readText(textPtr node)
		{
			node.set(readWstring());
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
