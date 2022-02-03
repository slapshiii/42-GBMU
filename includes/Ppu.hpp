#ifndef _PPU_HPP
#define _PPU_HPP

#include "utils.hpp"
#include "Gmbu.hpp"
#include "Lcd.hpp"

static const int LINES_PER_FRAME = 154;
static const int TICKS_PER_FRAME = 456;
static const int YRES = 144;
static const int XRES = 160;

typedef enum {
	FS_TILE,
	FS_DATA0,
	FS_DATA1,
	FS_IDLE,
	FS_PUSH
} fetchState;

typedef struct s_node {
	struct s_node	*next;
	uint32_t		data;
}	node;

typedef struct {
	node	*head;
	node	*tail;
	uint32_t	size;
}	queue;

typedef struct {
	fetchState	curFetchState;
	queue		pixelQueue;
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
	unsigned	h_flip: 1;
	unsigned	v_flip: 1;
	unsigned	dis_prio: 1;
} oam_obj;

typedef struct s_oamLineEntry {
	oam_obj	entry;
	struct s_oamLineEntry	*next;
}	oamLineEntry;

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

public:
	Ppu(/* args */);
	~Ppu();

	void	step();
	void	writeOam(uint16_t addr, uint8_t value);
	uint8_t	readOam(uint16_t addr);

	void	writeVram(uint16_t addr, uint8_t value);
	uint8_t	readVram(uint16_t addr);

	void	mode_oam();
	void	mode_xfer();
	void	mode_vblank();
	void	mode_hblank();
};

#endif