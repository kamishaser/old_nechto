#pragma once
#include "visualNode.h"
#include "visualConnection.h"
#include "connectionIterator.h"
#include "periodLimiter.h"

namespace nechto::ide
{
	
	using namespace std::chrono;
	
	glm::vec2 randomPos(glm::vec2 border)
	{
		return glm::vec2
		(
			static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / border.x)),
			static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / border.y))
		);
	}
	glm::vec2 randomOffset(float max)
	{
		return glm::vec2
		(
			static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / max/2))-max,
			static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / max/2))-max
		);
	}
	struct graph
	{
		class handler
		{
		public:
			std::shared_ptr<graph> nGraph;
			bool switchedOn = true;
			periodLimiter plim;
			//возвращает true, если произвёл изменения и false если нет
			
			handler(milliseconds minP, milliseconds maxP)
				:plim(minP, maxP) {}
			virtual void update() {};
			virtual ~handler() {};
		};
	public:
		std::map<nodePtr, visualNode> nodes;
		std::map<visualConnectionID, visualConnectionData> connections;

		/*
		visualNode& addNode(nodePtr v1);
		void resetNode(nodePtr v1);
		bool containsNode(nodePtr v1);
		visualNode& findNode(nodePtr v1);
		visualConnectionData& connect(visualConnectionID c1);
		void disconnect(visualConnectionID c1);
		bool connectionExist(visualConnectionID c1);
		visualConnectionData& findConnection(visualConnectionID c1);
		*/

		virtual bool addNode(nodePtr v1)
		{
			assert(v1.exist());
			visualNode temp(v1);
			temp.position += randomOffset(400);
			nodes.insert(std::move(std::pair<const nodePtr, visualNode>
				(std::move(temp.exCon), std::move(temp))));
			return true;			
		}
		
		void resetNode(nodePtr v1)
		{
			assert(v1.exist());
			assert(nodes.contains(v1));
			nodes.erase(v1);
			for (auto i = connections.begin(); i != connections.end(); ++i)
				if (i->first.first == v1 || i->first.second == v1)
					i = connections.erase(i);
		}
		bool containsNode(nodePtr v1)const
		{
			return nodes.contains(v1);
		}
		visualNode& findNode(nodePtr v1)
		{
			assert(v1.exist());
			assert(containsNode(v1));
			return nodes.at(v1);
		}
		
		virtual bool connect(visualConnectionID c1)
		{
			visualConnectionData temp;
			assert(containsNode(c1.first) && containsNode(c1.second));
			connections.emplace(c1, temp);
			//determineConnectionNumbers(c1);
			return true;
		}
		void disconnect(visualConnectionID c1)
		{
			assert(connections.contains(c1));
			connections.erase(c1);
		}
		bool connectionExist(visualConnectionID c1)
		{
			return connections.contains(c1);
		}
		visualConnectionData& findConnection(visualConnectionID c1)
		{
			assert(connectionExist(c1));
			return connections.at(c1);
		}
		virtual ~graph() {}

		/*void determineConnectionNumbers(visualConnectionID c1)
		{
			if (!connectionExist(c1))
				connect(c1);
			using nPair = visualConnectionData::oneConNumbers;
			std::vector<nPair>& vcn = connections.at(c1).connections;
			vcn.clear();
			connectionIterator i(c1.first, false);
			do
			{
				if (i.get() == c1.second)
					vcn.push_back(nPair(i.position, -1));

			} while (++i);
			i64 number = 0;
			do
			{
				if (i.get() == c1.second)
				{
					vcn[number].second = i.position;
					++number;
				}

			} while (++i);
			if (vcn.empty())
				disconnect(c1);
		}*/

		std::set<nodePtr> allNodeConnections(nodePtr v1)
		{
			std::set<nodePtr> con;
			if (!containsNode(v1))
				return con;
			for (auto i = connections.begin(); i != connections.end(); ++i)
			{
				if (i->first.first == v1)
					con.emplace(i->first.first);
				else
					if (i->first.second == v1)
						con.emplace(i->first.second);
			}
			return con;
		}
	};
}