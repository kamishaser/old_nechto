#pragma once
#include "namedExCon.h"
#include "connecter.h"

namespace nechto
{
	struct namedExConGroup :public namedExCon
	{
		namedExConGroup(const std::u16string& n)
			:namedExCon(n)
		{
			
			simplifiedNumNumConnect(get(), creator::createGroup(), 0, 0);
		}
		namedExConGroup(externalObjectNullPtr emptyExternalObject, 
			const std::u16string& n)
			:namedExCon(emptyExternalObject, n)
		{
			simplifiedNumNumConnect(get(), creator::createGroup(), 0, 0);
		}
		virtual ~namedExConGroup()
		{
			if (get().connection(0).exist())
				creator::deleteNode(get().connection(0));
		}
		nodePtr getGroup() const
		{
			return get().connection(0);
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
		/*получение указателя на namedExCon по объекту.
		Возвращает nullptr при несоответствии*/
		static namedExConGroup* getByNode(nodePtr v1)
		{
			if (!v1.exist())
				return nullptr;
			if (v1->getType() != node::ExternalObject)
				return nullptr;
			return dynamic_cast<namedExConGroup*>(v1->getData<externalObject*>());
		}
		const static std::u16string typeName;
		//const static staticNodeOperationSet methodSet;
		const static connectionRule cRule;
		virtual const std::u16string& getTypeName() const override
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
	const std::u16string namedExConGroup::typeName = u"namedExConGroup";
	const connectionRule namedExConGroup::cRule = connectionRule{};
}