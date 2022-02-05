#ifndef _PPU_HPP
#define _PPU_HPP

#include <list>

#include "utils.hpp"
#include "Cpu.hpp"
#include "Bus.hpp"
#include "Lcd.hpp"

static const int LINES_PER_FRAME = 154;
static const int TICKS_PER_LINE = 456;
static const int YRES = 144;
static const int XRES = 160;

typedef enum {
	FS_TILE,
	FS_DATA0,
	FS_DATA1,
	FS_IDLE,
	FS_PUSH
} fetchState;

typedef struct {
	fetchState	curFetchState;
	std::list<uint32_t>	pixelQueue;
	uint8_t		lineX;
	uint8_t		pushedX;
	uint8_t		fetchX;
	uint8_t		bgwFetchData[3];
	uint8_t		fetchEntryData[6];
	uint8_t		mapY;
	uint8_t		mapX;
	uint8_t		tileY;
	uint8_t		queueX;
}	pixelQueue;

//	Chap 2 1.7
typedef struct {
	uint8_t _y;
	uint8_t _x;
	uint8_t _tile;

	unsigned	clr_pal: 3;
	unsigned	char_bank: 1;
	unsigned	dmg_m_pal: 1;
	unsigned	h_flip: 1; //xflip
	unsigned	v_flip: 1; //yflip
	unsigned	dis_prio: 1;
} oam_obj;

typedef struct s_oamLineEntry {
	oam_obj	entry;
	struct s_oamLineEntry	*next;
}	oamLineEntry;

class Cpu;
class Bus;
class Ppu
{
public:

	oam_obj	_oamRam[40];
	uint8_t	_vram[0x2000];

	pixelQueue _pxQueue;

	uint8_t			_lineSpriteCount;
	oamLineEntry	*_lineSprites;
	oamLineEntry	_lineEntryArray[10];

	uint8_t	_fetchedEntryCount;
	oam_obj	_fetchedEntries[3];
	uint8_t	_winLine;

	uint32_t	_curFrame;
	uint32_t	_lineTicks;
	uint32_t	*_videoBuf;

	Lcd	_lcd;

private:
	Cpu	&_cpu;
	Bus	&_bus;

	static uint32_t	_targetFrameTime;
	static long		_prevFrameTime;
	static long		_startTimer;
	static long		_frameCount;

public:
	Ppu(Bus &b, Cpu &c);
	~Ppu();

	void	step();

	uint8_t	read(uint16_t addr);
	void	write(uint16_t addr, uint8_t data);

	void	writeOam(uint16_t addr, uint8_t value);
	uint8_t	readOam(uint16_t addr);

	void	writeVram(uint16_t addr, uint8_t value);
	uint8_t	readVram(uint16_t addr);

	void	incrementLy();
	void	mode_oam();
	void	mode_xfer();
	void	mode_vblank();
	void	mode_hblank();

	void	loadWinTile();
	void	loadSptTile();
	void	loadSptData(uint8_t offset);

private:
	bool isWinvisible();
	bool queueAdd();
	void pxlProcess();
	void pxlFetch();
	void pxlPush();

	uint32_t fetchSptPx(int bit, uint32_t color, uint8_t bgColor);
};

#endif