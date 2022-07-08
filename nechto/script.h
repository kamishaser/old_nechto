#pragma once
#include "node.h"
#include "checkAndStep.h"

#include <set>
#include <stack>


namespace nechto
{
	class script
	{
		nodePtr startNode;
	public:
		script(nodePtr sNode)
			:startNode(sNode)
		{}
		void setStartNode(const nodePtr sNode)
		{ startNode = sNode; }

		void operator()()const
		{
			nodePtr temp = startNode;
			//assert(isCorrect());
			while (step(temp));
		}

		std::set<nodePtr>&& nodesInScript() const;
		
		std::set<nodePtr>&& errorList() const
		{
			std::set<nodePtr> nodeSet = nodesInScript();
			std::set<nodePtr> errorSet;

			for (auto i = nodeSet.begin(); i != nodeSet.end(); ++i)
			{
				if (!i->isCorrect())
					errorSet.emplace(*i);
			}
			return std::move(errorSet);
		}
		bool isCorrect() const
		{
			std::set<nodePtr> nodeSet = nodesInScript();
			for (auto i = nodeSet.begin(); i != nodeSet.end(); ++i)
			{
				std::cout  << to_string(*i) << std::endl;
				if (!i->isCorrect())
					return false;
			}
			return true;
		}
		

	};


	/////////////////////////////////////////////////////////////////////////////////////////////////
	std::set<nodePtr>&& script::nodesInScript() const
	{
		std::set<nodePtr> nodeSet;
		std::stack<nodePtr> branches;
		nodePtr current = startNode;
		nodePtr next;
		nodeSet.emplace(current);

		std::function<bool(nodePtr, char)> isConnactionMarked
		{ [&](nodePtr v1, char conNum)
			{
				if (!v1->hasConnection(conNum))
					return true;
				if (nodeSet.contains(v1->connection[conNum].load()))
					return true;
				return false;
			} };
		std::function<void()> getNextFromBranches
		{ [&]()
			{
				while (!branches.empty())
				{
					next = branches.top();
					branches.pop();
					if (!nodeSet.contains(next))
						return;
				}
				next = nullNodePtr;
			} };
		do
		{
			if (current->getType() == node::ConditionalBranching)
			{
				if (isConnactionMarked(current, 2))
				{
					if (isConnactionMarked(current, 3))
						getNextFromBranches();
					else
						next = current->connection[3].load();
				}
				else
				{
					next = current->connection[2].load();
					if (!isConnactionMarked(current, 3))
						branches.push(current->connection[3].load());
				}

			}
			else
			{
				if (isConnactionMarked(current, 3))
					getNextFromBranches();
				else
					next = current->connection[3].load();
			}
			for (int i = 0; i < 4; i++)
			{
				if (current->hasConnection(i))
					nodeSet.emplace(current->connection[i]);
			}
			
		} while ((current = next).exist());
		return std::move(nodeSet);
	}
}