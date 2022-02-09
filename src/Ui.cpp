#include "Ui.hpp"

extern Gbmu gbmu;

void	checkEvent(sf::Event e, sf::RenderWindow &win, sf::RenderWindow &dbg) {
	switch (e.type)
	{
	case sf::Event::Closed:
		gbmu.stop();
		if (dbg.isOpen())
			dbg.close();
		win.close();
		break;
	
	default:
		break;
	}
}