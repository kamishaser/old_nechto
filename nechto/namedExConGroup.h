#pragma once
#include "namedExCon.h"
#include "group.h"

namespace nechto
{
	struct namedExConGroup :public namedExCon
	{
		namedExConGroup(const std::wstring& n)
			:namedExCon(n)
		{
			NumNumConnect(get(), newNode(node::Group), 0, 0);
		}
		namedExConGroup(nodePtr emptyExternalObject, const std::wstring& n)
			:namedExCon(emptyExternalObject, n)
		{
			NumNumConnect(get(), newNode(node::Group), 0, 0);
		}
		virtual ~namedExConGroup()
		{
			if (getConnection(0).exist())
				deleteNode(getConnection(0));
		}
		nodePtr getGroup() const
		{
			return getConnection(0);
		}
		bool contains(nodePtr v1) const
		{
			return group::contains(getGroup(), v1);
		}
		void addNode(nodePtr v1) const
		{
			IterHubConnect(group::firstEmptyPort(getGroup()), v1);
		}
		void addNode(hubIterator hi) const
		{
			IterIterConnect(group::firstEmptyPort(getGroup()), hi);
		}
		i64 numberOfVNodes() const
		{
			return group::numberOfMembers(getGroup());
		}
		/*��������� ��������� �� namedExCon �� �������.
		���������� nullptr ��� ��������������*/
		static namedExConGroup* getByNode(nodePtr v1)
		{
			if (!v1.exist())
				return nullptr;
			if (v1->getType() != node::ExternalObject)
				return nullptr;
			return dynamic_cast<namedExConGroup*>(v1->getData<externalObject*>());
		}
		const static std::wstring typeName;
		//const static staticNodeOperationSet methodSet;
		const static connectionRule cRule;
		virtual const std::wstring& getTypeName() const override
		{
			return typeName;
		}
		/*virtual const operation& getMethod(char number)const override
		{
			return methodSet.getOperation(number);
		}*/
		virtual const conRule& getConnectionRule()const override
		{
			return cRule;
		}
	};
	const std::wstring namedExConGroup::typeName = L"namedExConGroup";
	const connectionRule namedExConGroup::cRule = connectionRule{};
}