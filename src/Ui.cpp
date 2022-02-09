#include "Ui.hpp"

extern Gbmu gbmu;
extern sf::RenderWindow _dbgWindow;
extern sf::Texture _dbgTexture;
extern sf::RenderWindow _mainWindow;
extern sf::RenderTexture _mainTexture;
extern const size_t	scale;

void	checkEvent(sf::Event e) {
	switch (e.type)
	{
	case sf::Event::Closed:
		gbmu.stop();
		if (_dbgWindow.isOpen())
			_dbgWindow.close();
		_mainWindow.close();
		break;
	
	default:
		break;
	}
}

static uint32_t tile_color[4] = {0xFFFFFFFF, 0xFFAAAAAA, 0xFF555555, 0xFF000000};
static void displayTile(uint16_t addr, uint16_t tileNum, int x, int y) {
	for (int tileY = 0; tileY < 16; tileY += 2) {
		uint8_t b1 = gbmu.mem_read(addr + (tileNum * 16) + tileY);
		uint8_t b2 = gbmu.mem_read(addr + (tileNum * 16) + tileY + 1);
		for (int bit = 7; bit >= 0; bit--) {
			uint8_t hi = !!(b1 & (1 << bit)) << 1;
			uint8_t lo = !!(b2 & (1 << bit));
			uint8_t color = hi | lo;
			std::array<uint32_t, scale * scale> pxlArray;
			pxlArray.fill(tile_color[color]);
			_dbgTexture.update(
				&pxlArray,
				scale, scale,
				x + ((7 - bit) * scale),
				y + (tileY / 2 * scale)
			);
		}
	}
}

void	update_dbgWin() {
	int xText = 0;
	int yText = 0;
	int tileNum = 0;
	uint16_t addr = 0x8000;

	for (int y = 0; y < 24; ++y) {
		for (int x = 0; x < 16; ++x) {
			displayTile(addr, tileNum, xText + (x * scale), yText + (y * scale));
			xText += (8 * scale);
			tileNum++;
		}
		yText += (8 * scale);
		xText = 0;
	}
	_dbgWindow.clear();
	_dbgWindow.draw(_dbgTexture);
	_dbgWindow.display();
}