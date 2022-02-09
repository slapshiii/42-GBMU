#include "Gbmu.hpp"
#include "Ui.hpp"

#include <signal.h>
#include <SFML/Graphics.hpp>

Gbmu gbmu;

const size_t	scale = 5;
const size_t	scrnW = 1024;
const size_t	scrnH = 768;
bool dbg_mode = true;

sf::RenderWindow _dbgWindow;
sf::RenderTexture _dbgTexture;
sf::RenderWindow _mainWindow;
sf::RenderTexture _mainTexture;


int main()
{
	if (dbg_mode) {
		_dbgWindow.create(
			sf::VideoMode(160 * scale, 144 * scale, 32),
			"Dbg_Gbmu",
			sf::Style::Titlebar
		);
		_dbgWindow.setKeyRepeatEnabled(false);
		_dbgWindow.setFramerateLimit(60);
		_dbgTexture.create(160 * scale, 144 * scale);
	}
	_mainWindow.create(
		sf::VideoMode(scrnW, scrnH, 32),
		"Gbmu",
		sf::Style::Titlebar | sf::Style::Close
	);
	_mainWindow.setKeyRepeatEnabled(false);
	_mainWindow.setFramerateLimit(60);
	_mainTexture.create(scrnW, scrnH);
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
		update_dbgWin();
	}

	return (0);
}