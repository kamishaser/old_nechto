#pragma once
#include "visualNode.h"
#include "visualConnection.h"
#include "connectionIterator.h"

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
			graph* nGraph;
			bool switchedOn = true;
			//���������� true, ���� ������� ��������� � false ���� ���
			
			virtual void update(milliseconds timiInterval) {};
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

		visualNode& addNode(nodePtr v1)
		{
			visualNode temp;
			temp.position = randomOffset(100);
			return nodes.emplace(v1, temp).first->second;
			
		}
		void resetNode(nodePtr v1)
		{
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
			assert(containsNode(v1));
			return nodes.at(v1);
		}
		
		visualConnectionData& connect(visualConnectionID c1)
		{
			visualConnectionData temp;
			assert(containsNode(c1.first) && containsNode(c1.second));
			visualConnectionData& result = 
				connections.emplace(c1, temp).first->second;
			determineConnectionNumbers(c1);
			return result;
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

		void determineConnectionNumbers(visualConnectionID c1)
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
			int64_t number = 0;
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
		}
	};
}