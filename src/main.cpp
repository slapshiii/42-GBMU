#include "Gbmu.hpp"
#include "Ui.hpp"

#include <signal.h>
#include <SFML/Graphics.hpp>

Gbmu gbmu;

const size_t	scale = 5;
bool dbg_mode = true;

int main()
{
	sf::RenderWindow _dbgWindow;
	if (dbg_mode) {
		_dbgWindow.create(sf::VideoMode(80 * scale, 150 * scale, 32), "Dbg_Gbmu", sf::Style::Titlebar);
		_dbgWindow.setKeyRepeatEnabled(false);
		_dbgWindow.setFramerateLimit(60);
	}
	sf::RenderWindow _mainWindow(sf::VideoMode(160 * scale, 144 * scale, 32), "Gbmu", sf::Style::Titlebar | sf::Style::Close);
	_mainWindow.setKeyRepeatEnabled(false);
	_mainWindow.setFramerateLimit(60);
	while (_mainWindow.isOpen()) {
		sf::Event e;
		while (_mainWindow.pollEvent(e)) {
			checkEvent(e, _mainWindow, _dbgWindow);
		}
		_mainWindow.clear();
		
		//_mainWindow.draw();
		_mainWindow.display();
		if (_dbgWindow.isOpen()) {
			_dbgWindow.clear();
			_dbgWindow.display();
		}
	}

	return (0);
}