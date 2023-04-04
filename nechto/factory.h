#pragma once

#include "connecter.h"
#include "creator.h"

#include "group.h"
#include "portSearch.h"
#include "variablePtr.h"
#include "struct.h"
#include "entity.h"
#include "text.h"

namespace nechto
{
	enum class connectionType
	{
		N0,
		N1,
		N2,
		N3,
		Hub,
		Group
	};
	class sType//информация для создания ноды в фабрике
	{
		uchar type = 0;
		uchar subtype = 0;

		i64 data = 0;//данные. Используются в зависимости от типа
		constexpr sType() {}//прямой вызов конструктора недоступен, т. к. опасен
		constexpr sType(uchar t, uchar s)
			:type(t), subtype(s), data(0) {}
		template<class tCon>
		constexpr sType(uchar t, uchar s, tCon* d)
			: type(t), subtype(s)
		{
			static_assert(sizeof(tCon) <= 8);
			data = *reinterpret_cast<i64*>(d);
		}
	public:
		constexpr static sType i64v(i64 value)
		{
			return sType(nodeT::Variable, variableT::I64, &value);
		}
		constexpr static sType f64v(f64 value)
		{
			return sType(nodeT::Variable, variableT::F64, &value);
		}
		constexpr static sType text(bool owning)
		{
			return sType(nodeT::Text, owning);
		}
		constexpr static sType entity(uchar subtype)
		{
			return sType(nodeT::Entity, subtype);
		}
		constexpr static sType structNode(uchar subtype, structData data)
		{
			return sType(nodeT::Struct, subtype, &data);
		}
		constexpr static sType operation(uchar type, uchar subtype)
		{
			assert(isOperation(type));
			return sType(type, subtype);
		}
		constexpr static sType portPointerNode(i64 globalPos)
		{
			return sType(nodeT::Pointer, pointerT::PortPointer, &globalPos);
		}
		constexpr static sType groupPointerNode(i64 globalPos)
		{
			return sType(nodeT::Pointer, pointerT::GroupPointer, &globalPos);
		}
		constexpr static sType weakGroup()
		{
			return sType(nodeT::Group, groupT::weak);
		}
		constexpr static sType strongGroup()
		{
			return sType(nodeT::Group, groupT::strong);
		}
		constexpr static sType deleted()
		{
			return sType(nodeT::Deleted, 0);
		}
		nodePtr create() const
		{
			nodePtr node = creator::createNode(type, subtype);
			switch (type)
			{
			case nechto::nodeT::Struct:
				node.node()->data = data;//надеюсь так можно
				break;
			case nechto::nodeT::Pointer:
				assert(false);//потом доделаю
				break;
			case nechto::nodeT::Path:
				node.node()->data = data;
				break;
			case nechto::nodeT::Variable:
				node.node()->data = data;
				break;
			default:
				break;
			}
			return node;
		}
	};
	struct fstep
	{
		struct connection
		{
			std::wstring name;
			connectionType firstConType = connectionType::Hub;
			connectionType secondConType = connectionType::Hub;

			static connectionType getConTypeByWchar(const wchar_t ch)
			{
				switch (ch)
				{
				case L'0':
					return connectionType::N0;
				case L'1':
					return connectionType::N1;
				case L'2':
					return connectionType::N2;
				case L'3':
					return connectionType::N3;
				case L'h':
					return connectionType::Hub;
				case L'g':
					return connectionType::Group;
				default:
					assert(false);
				}
			}
			static wchar_t getWcharByConType(connectionType ct)
			{
				switch (ct)
				{
				case nechto::connectionType::N0:
					return L'0';
				case nechto::connectionType::N1:
					return L'1';
				case nechto::connectionType::N2:
					return L'2';
				case nechto::connectionType::N3:
					return L'3';
				case nechto::connectionType::Hub:
					return L'h';
				case nechto::connectionType::Group:
					return L'g';
				default:
					assert(false);
				}
			}
			connection(const std::wstring& str)
				:name(str.substr(3)),
				firstConType(getConTypeByWchar(str[0])),
				secondConType(getConTypeByWchar(str[1]))
			{
				if (str.size() < 3 || str[2] != '/')
					assert(false);
			}
			connection() {}
		};

		std::wstring nodeName;
		sType node;
		std::vector<connection> connections;


		fstep(const std::wstring& nName, sType n,
			std::initializer_list<connection> nConnections)
			:nodeName(nName), node(n), connections(nConnections.size())
		{
			for (size_t i = 0; i < nConnections.size(); ++i)
			{
				connections[i] = nConnections.begin()[i];
			}
		}
		fstep()
			:node(sType::deleted()) {}
	};
	struct serialPlanPart;
	struct serialPlan : public std::vector<fstep>
	{
		static serialPlan nullPlan;
		
		constexpr serialPlan(nullptr_t) {}
		constexpr serialPlan() {}
		constexpr serialPlan(
			std::initializer_list<serialPlanPart> plist,
			std::initializer_list<fstep> slist);
		constexpr serialPlan(const serialPlanPart& sn0,
			const serialPlanPart& sn1, const serialPlanPart& sn2);
		constexpr serialPlan(std::initializer_list<fstep> flist)
			: std::vector<fstep>{ flist } {}

		static void fill(iterator& iter, const serialPlanPart& part);
	};
	serialPlan serialPlan::nullPlan{ nullptr };
	struct serialPlanPart
	{
		const std::wstring partName;
		const serialPlan plan;
		constexpr serialPlanPart(nullptr_t)
			: partName(L""), plan(serialPlan::nullPlan) {}
		constexpr serialPlanPart(const std::wstring& name, const serialPlan& plan = serialPlan::nullPlan)
			: partName(name), plan(plan) {}
		constexpr serialPlanPart(const std::wstring& nName, sType n,
			std::initializer_list<fstep::connection> nConnections = {})
			: partName(nName), plan{ { L"", n, nConnections } } {}
		constexpr bool exist() const { return !plan.empty(); }
	}; 
	constexpr serialPlan::serialPlan(
		std::initializer_list<serialPlanPart> plist,
		std::initializer_list<fstep> slist)
	{
		for (auto& part : plist)
		{
			assert(!part.partName.empty());
			for (auto& i : part.plan)
			{
				push_back(i);
				if (i.nodeName.empty())
					back().nodeName = part.partName;
				else
					back().nodeName =
					part.partName + L"." + i.nodeName;
				for (auto& con : back().connections)
				{
					con.name = con.name.empty() ?
						part.partName : part.partName + L"." + con.name;
				}
			}
		}
		for (auto& i : slist)
		{
			push_back(i);
		}
	}
	constexpr serialPlan::serialPlan(const serialPlanPart& sn0 = 
		serialPlanPart(nullptr),
		const serialPlanPart& sn1 = serialPlanPart(nullptr), 
		const serialPlanPart& sn2 = serialPlanPart(nullptr))
		:std::vector<fstep>(1 + sn0.plan.size() + sn1.plan.size() + sn2.plan.size())
	{
		back() = fstep(L"", sType::structNode(0, structData()), {});
		iterator iter = begin();
		if (sn0.exist())
		{
			back().connections.push_back(L"03/" + sn0.partName);
			fill(iter, sn0);
		}
		if (sn1.exist())
		{
			back().connections.push_back(L"13/" + sn1.partName);
			fill(iter, sn1);
		}
		if (sn2.exist())
		{
			back().connections.push_back(L"23/" + sn2.partName);
			fill(iter, sn2);
		}
	}
	void serialPlan::fill(iterator& iter, const serialPlanPart& part)
	{
		assert(!part.partName.empty());
		for (auto& iterOld : part.plan)
		{
			*iter = iterOld;
			if (iter->nodeName.empty())
				iter->nodeName = part.partName;
			else
				iter->nodeName =
				part.partName + L"." + iter->nodeName;
			for (auto& con : iter->connections)
			{
				con.name = con.name.empty() ?
					part.partName : part.partName + L"." + con.name;
			}
			++iter;
		}
	}

	class factory
	{
	public:
		std::map<std::wstring, nodePtr> finishedParts;

		virtual void clear()
		{
			finishedParts.clear();
		}
		virtual void fabricate(const serialPlan& plan, nodePtr node = nullptr)
		{
			assert(!finishedParts.contains(L""));
			if (node != nullptr)
				finishedParts.emplace(L"", node);
			for (auto i = plan.cbegin(); i != plan.cend(); ++i)
			{
				const std::wstring& nodeName = i->nodeName;
				if (finishedParts.contains(nodeName))
				{
					fabricateConnections(finishedParts[nodeName], *i);
				}
				else
				{
					nodePtr node = i->node.create();
					fabricateConnections(node, *i);
					finishedParts.emplace(i->nodeName, node);
				}
			}
		}
		
		void clearAndFabricate(const serialPlan& plan, nodePtr node = nullptr)
		{
			clear();
			fabricate(plan, node);
		}
		void fill(const std::wstring& name, nodePtr node)
		{
			if (node.exist())
				finishedParts.emplace(name, node);
		}
	private:
		void fabricateConnections(nodePtr node, const fstep& step)
		{
			for (auto con = step.connections.cbegin(); con != step.connections.cend(); ++con)
			{
				if (!finishedParts.contains(con->name))
					continue;
				nodePtr node2 = finishedParts[con->name];
				fabricateConnection(node, node2, con->firstConType, con->secondConType);
			}
		}
		void fabricateConnection(existing<nodePtr> fNode, existing<nodePtr> sNode, connectionType c1, connectionType c2)
		{
			connecter::connect(getPortPtr(fNode, c1), getPortPtr(sNode, c2));
		}
		bool hasConnection(existing<nodePtr> fNode, existing<nodePtr> sNode, connectionType c1, connectionType c2)
		{
			return hasOneSideConnection(fNode, sNode, c1) && hasOneSideConnection(sNode, fNode, c2);
		}
		bool hasOneSideConnection(existing<nodePtr> fNode, existing<nodePtr> sNode, connectionType con)
		{
			if (con == connectionType::Hub)
			{
				portPointer iter(fNode);
				do
				{
					if (iter.get() == sNode)
						return true;
				} while (iter.stepForward());
				return false;
			}
			else if (con == connectionType::Group)
			{
				if (fNode.type() != nodeT::Group)
					return false;
				return (groupOperations::contains(fNode, sNode));
			}
			else
			{
				return (getPortPtr(fNode, con).get() == sNode);
			}
		}
		pointer getPortPtr(nodePtr node, connectionType con)
		{
			switch (con)
			{
			case nechto::connectionType::N0:
				return pointer(node, hubPosPair(node, 0));
			case nechto::connectionType::N1:
				return pointer(node, hubPosPair(node, 1));
			case nechto::connectionType::N2:
				return pointer(node, hubPosPair(node, 2));
			case nechto::connectionType::N3:
				return pointer(node, hubPosPair(node, 3));
			case connectionType::Hub:
				return firstEmptyHubPort(node);
			case connectionType::Group:
				assert(node.type() == nodeT::Group);
				return firstEmptyGroupPort(node);
			default:
				assert(false);
			}
		}
	};
	static nodePtr fabricate(const serialPlan& plan, nodePtr node = nullptr)
	{
		factory fact;
		fact.fabricate(plan, node);
		return fact.finishedParts.at(L"");
	}
}