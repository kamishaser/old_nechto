export module display;
import "SFML/Graphics.hpp";
import "entity.h";

namespace nechto::ide
{
	class display : public singleConnectedEntity
	{
		sf::Window window;
		
	};
}