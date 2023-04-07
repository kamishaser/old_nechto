#pragma once
#include "entity.h"
#include "structure.h"
#include "creator.h"
#include "visual.h"
#include "ideStructurePack.h"
#include <queue>

namespace nechto::ide
{
	class windowEntity : public singleConnectedEntity
	{
	public:
		sf::RenderWindow window;
		glm::vec2 workBoardViewPos = { 500, 500 };
		float workBoardViewScale = 1;
		static std::queue<wchar_t> inputTextEvents;//список введённых символов
		windowEntity()
			:window(sf::VideoMode(1000, 1000), "nechtoIDE")
		{
			eConnect(creator::createEntity(entityT::singleConnection));
		}
		bool connectToIde(nodePtr ideNode)
		{
			nodePtr grNode = sPack::nechtoIde::windowGroup / ideNode;
			if (!groupPtr::match(grNode))
				return false;
			PointerNumConnect(backGroupPort(grNode), node(), 3);
			return true;
		}
		bool update()
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				switch (event.type)
				{
				case sf::Event::EventType::Closed:
					return false;
					break;
				case sf::Event::EventType::TextEntered:
					//по хорошему тут должно вызываться событие записи символа
					if(window.hasFocus())
						inputTextEvents.push(event.text.unicode);
				default:
					break;
				}
			}
			return true;
		}
		void interfaceBoardView()
		{
			glm::vec2 size = GLM_SFML(window.getSize());
			window.setView(sf::View(GLM_SFML(size / 2.f),
				GLM_SFML(size)));
		}
		void workBoardView()
		{
			glm::vec2 size = GLM_SFML(window.getSize());
			size *= workBoardViewScale;
			window.setView(sf::View(GLM_SFML(workBoardViewPos),
				GLM_SFML(size)));
		}
		glm::vec2 getPointAtBoard(glm::vec2 point)
		{
			return GLM_SFML(window.mapPixelToCoords(GLM_SFML<int>(point)));
		}
	};
	std::queue<wchar_t> windowEntity::inputTextEvents{};
}