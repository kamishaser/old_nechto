#pragma once
#include "visualNode.h"
#include "GLM/glm.hpp"
#include "textOut.h"
#include <chrono>

namespace nechto::ide
{
	
	using namespace std::chrono;
	milliseconds currentTime()
	{
		return duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	}
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
	struct nodeBoard
	{
	public:
		class handler
		{
		public:
			nodeBoard* nBoard;
		
			//возвращает true, если произвёл изменения и false если нет
			virtual void update(milliseconds timiInterval) {};
			virtual ~handler() {};
		};
	private:
		std::vector<std::shared_ptr<handler>> handlerSet;
	public:
		std::vector<sharedVisualNode> vNode;
		
		glm::vec2 boardSize;
		milliseconds lastUp;

		
		nodeBoard(glm::vec2 size)
			:boardSize(size), lastUp(currentTime()) {}

		void addHandler(std::shared_ptr<handler> han)
		{
			std::cout << "update" << std::endl;
			han->nBoard = this;
			handlerSet.push_back(han);
		}
		void update()
		{
			milliseconds timeInterval = currentTime() - lastUp;
			if (timeInterval > 500ms)
				timeInterval = 500ms;
			for (auto i = handlerSet.begin(); i != handlerSet.end(); ++i)
			{
				if (*i)
					(*i)->update(timeInterval);
				else
					handlerSet.erase(i);
			}
			lastUp = currentTime();
			std::cout << vNode[0]->position.x << ' ' << vNode[0]->position.y << std::endl;
		}
		bool contains(nodePtr v1)
		{
			if (!v1.exist())
				return false;
			return (std::find_if(vNode.begin(), vNode.end(),
				[&](const sharedVisualNode v2)->bool {return nodePtr(*v2) == v1; })
				!= vNode.end());
		}
		sharedVisualNode operator[](nodePtr v1)
		{
			assert(contains(v1));
			return *std::find_if(vNode.begin(), vNode.end(),
				[&](const sharedVisualNode v2)->bool {return nodePtr(*v2) == v1; });
		}
		sharedVisualNode add(nodePtr v1, glm::vec2 pos = glm::vec2(500, 500), float size = 50)
		{
			sharedVisualNode vn1;
			
			if (!contains(v1))
			{
				vn1 = std::make_shared<visualNode>(v1, pos + randomOffset(400), size);
				vNode.push_back(vn1);
				auto vn1Con = allNodeConnections(*vn1);
				for (int64_t i = 0; i < static_cast<int64_t>(vn1Con.size()); ++i)
				{
					if (contains(vn1Con[i]))
					{
						auto backNumber = getConnectionNumber(vn1Con[i], v1);
						vn1->connections.emplace(i, this->operator[](vn1Con[i]));
						

						std::cout << "the connection " 
							<< to_string(*vn1) << ' ' << to_string(vn1Con[i])
							<< " is set ("<< i << ' ' << backNumber << ")" << std::endl;

						this->operator[](vn1Con[i])->connections.emplace(backNumber, vn1);
					}
				}
				return vn1;
			}
			return (*this)[v1];
		}

	};
}