#include "Ppu.hpp"

Ppu::Ppu(Bus &b, Cpu &c) : _cpu(c), _bus(b){
    this->_videoBuf = (uint32_t *)malloc(YRES * XRES * (sizeof(32)));
}
Ppu::~Ppu(){
    free(this->_videoBuf);
}

void	Ppu::reset() {
	this->_curFrame = 0;
    this->_lineTicks = 0;
	this->_pxQueue.lineX = 0;
    this->_pxQueue.pushedX = 0;
    this->_pxQueue.fetchX = 0;
    this->_pxQueue.curFetchState = FS_TILE;
	//this->_videoBuf;
    this->_lineSprites = 0;
    this->_fetchedEntryCount = 0;
    this->_winLine = 0;
    LCDS_MODE_SET(MODE_OAM);
}

uint8_t	Ppu::read(uint16_t addr) {
	if (BETWEEN(addr, 0xA000, 0xBFFF))
		return (readVram(addr));
	else if (BETWEEN(addr, 0xFE00, 0xFE9F))
		return (readOam(addr));
	else if (BETWEEN(addr, 0xFF40, 0xFF4B))
		return (_lcd.read(addr));
	return 0;
}
void	Ppu::write(uint16_t addr, uint8_t data) {
	if (BETWEEN(addr, 0xA000, 0xBFFF))
		return (writeVram(addr, data));
	else if (BETWEEN(addr, 0xFE00, 0xFE9F))
		return (writeOam(addr, data));
	else if (BETWEEN(addr, 0xFF40, 0xFF4B))
		return (_lcd.write(addr, data));
}
void	Ppu::step() {
    ++this->_lineTicks;
    switch (this->_lcd.lcds & 0b11)
    {
    case MODE_OAM:
        this->mode_oam();
        break;
    case MODE_XFER:
        this->mode_xfer();
        break;
    case MODE_VBLANK:
        this->mode_vblank();
        break;
    case MODE_HBLANK:
        this->mode_hblank();
        break;
    }
}

void	Ppu::writeOam(uint16_t addr, uint8_t value) {
    if (addr >= 0xFE00) {
        addr -= 0xFE00;
    }
    uint8_t *p = (uint8_t *)(this->_oamRam);
    p[addr] = value;
}
uint8_t	Ppu::readOam(uint16_t addr) {
    if (addr >= 0xFE00) {
        addr -= 0xFE00;
    }
    uint8_t *p = (uint8_t *)(this->_oamRam);
    return (p[addr]);
}

void	Ppu::writeVram(uint16_t addr, uint8_t value) {
    this->_vram[addr - 0x8000] = value;
}
uint8_t	Ppu::readVram(uint16_t addr) {
    return this->_vram[addr - 0x8000];
}

void	Ppu::mode_oam() {
    if (this->_lineTicks >= 80) {
        LCDS_MODE_SET(MODE_XFER);
        this->_pxQueue.curFetchState = FS_TILE;
        this->_pxQueue.lineX = 0;
        this->_pxQueue.fetchX = 0;
        this->_pxQueue.pushedX = 0;
        this->_pxQueue.queueX = 0;
    }
    if (this->_lineTicks == 1) {
        this->_lineSprites = 0;
        this->_lineSpriteCount = 0;
        //this->loadLineSprites(); TODO
    }
}
void	Ppu::mode_xfer() {
    //pipe_process(); TODO
    if (this->_pxQueue.pushedX >= XRES) {
        //pipe_queueReset(); TODO
        LCDS_MODE_SET(MODE_HBLANK);
        if (LCDS_STAT_INT(SS_HBLANK)) {
            _cpu.requestInt(IT_LCD_STAT);
        }
    }
}
void	Ppu::mode_vblank() {
    if (this->_lineTicks >= TICKS_PER_LINE) {
        incrementLy();
        if (this->_lcd.ly >= LINES_PER_FRAME) {
            LCDS_MODE_SET(MODE_OAM);
            this->_lcd.ly = 0;
            this->_winLine = 0;
        }
        this->_lineTicks = 0;
    }
}

uint32_t Ppu::_targetFrameTime = 1000/60;
long Ppu::_prevFrameTime = 0;
long Ppu::_startTimer = 0;
long Ppu::_frameCount = 0;

uint32_t get_ticks(){
	return 0;
}

void Ppu::mode_hblank() {
	if (_lineTicks >= TICKS_PER_LINE) {
		incrementLy();
		if (_lcd.ly >= YRES) {
			LCDS_MODE_SET(MODE_VBLANK);
			_cpu.requestInt(IT_VBLANK);
			if (LCDS_STAT_INT(SS_VBLANK)) {
				_cpu.requestInt(IT_LCD_STAT);
			}
			++_curFrame;
			uint32_t end = get_ticks();
			uint32_t frameTime = end - _prevFrameTime;
			if (frameTime < _targetFrameTime)
				usleep(_targetFrameTime - frameTime);
			if (end - _startTimer >= 1000) {
				uint32_t fps = _frameCount;
				_startTimer = end;
				_frameCount = 0;
				std::cerr << "FPS: " << fps << std::endl;
				// SAVE TODO
			}
			_frameCount++;
			_prevFrameTime = get_ticks();
		} else {
			LCDS_MODE_SET(MODE_OAM);
		}
		_lineTicks = 0;
	}
}
bool Ppu::isWinvisible() {
	return (LCDC_WIN_ENABLE
		&& _lcd.winX >= 0 && _lcd.winX <= 166
		&& _lcd.winY >= 0 && _lcd.winY < YRES);
}
void Ppu::incrementLy() {
	if (isWinvisible() && _lcd.ly >= _lcd.winY
		&& _lcd.ly < _lcd.winY + YRES)
		_winLine++;
	this->_lcd.ly++;
	if (this->_lcd.ly == this->_lcd.lyComp)
	{
		LCDS_LYC_SET(1);
		if (LCDS_STAT_INT(SS_LYC)) {
			_cpu.requestInt(IT_LCD_STAT);
		}
	} else {
		LCDS_LYC_SET(0);
	}
}

void Ppu::pxlProcess(){
	_pxQueue.mapY = _lcd.ly + _lcd.scrollY;
	_pxQueue.mapX = _pxQueue.fetchX + _lcd.scrollX;
	_pxQueue.tileY = ((_lcd.ly + _lcd.scrollY) % 8) * 2;
	if (!(_lineTicks & 1))
		pxlFetch();
}
void Ppu::pxlFetch() {
	switch (_pxQueue.curFetchState)
	{
	case FS_TILE: {
		_fetchedEntryCount = 0;
		if (LCDC_BGW_ENABLE) {
			_pxQueue.bgwFetchData[0] = _bus.read(
				LCDC_BG_MAP_AREA
				+ (_pxQueue.mapX / 8)
				+ ((_pxQueue.mapY / 8) * 32)
			);
			if (LCDC_BGW_DATA_AREA == 0x8800)
				_pxQueue.bgwFetchData[0] += 128;
			loadWinTile();
		}
		if (LCDC_OBJ_ENABLE && _lineSprites)
			loadSptTile();
		_pxQueue.curFetchState = FS_DATA0;
		_pxQueue.fetchX += 8;
	} break;
	
	case FS_DATA0: {
		_pxQueue.bgwFetchData[1] = _bus.read(
			LCDC_BGW_DATA_AREA
			+ (_pxQueue.bgwFetchData[0] * 16)
			+ _pxQueue.tileY
		);
		loadSptData(0);
		_pxQueue.curFetchState = FS_DATA1;
	} break;
	case FS_DATA1: {
		_pxQueue.bgwFetchData[2] = _bus.read(
			LCDC_BGW_DATA_AREA
			+ (_pxQueue.bgwFetchData[0] * 16)
			+ _pxQueue.tileY + 1
		);
		loadSptData(1);
		_pxQueue.curFetchState = FS_IDLE;
	} break;
	case FS_IDLE: {
		_pxQueue.curFetchState = FS_PUSH;
	} break;
	case FS_PUSH: {
		if (queueAdd())
		_pxQueue.curFetchState = FS_TILE;
	} break;
	}
}
void Ppu::pxlPush() {
	if (_pxQueue.pixelQueue.size() > 8) {
		uint32_t pxData = _pxQueue.pixelQueue.front();
		_pxQueue.pixelQueue.pop_front();
		if (_pxQueue.lineX >= (_lcd.scrollX % 8)) {
			_videoBuf[_pxQueue.pushedX + (_lcd.ly * XRES)] = pxData;
			_pxQueue.pushedX++;
		}
		_pxQueue.lineX++;
	}
}
bool Ppu::queueAdd() {
	if (_pxQueue.pixelQueue.size() > 8) {
		return false;
	}
	int x = _pxQueue.fetchX - (8 - (_lcd.scrollX % 8));
	for (int i = 0; i < 8; ++i) {
		int bit  = 7 - i;
		uint8_t hi = !!(_pxQueue.bgwFetchData[1] & (1 << bit));
		uint8_t lo = !!(_pxQueue.bgwFetchData[2] & (1 << bit)) << 1;
		uint32_t color = _lcd.bgColor[hi | lo];
		if (!LCDC_BGW_ENABLE)
			color = _lcd.bgColor[0];
		if (!LCDC_OBJ_ENABLE)
			color = fetchSptPx(bit, color, hi | lo);
		if (x >= 0) {
			_pxQueue.pixelQueue.push_back(color);
			_pxQueue.queueX++;
		}
	}
	return true;
}

uint32_t Ppu::fetchSptPx(int bit, uint32_t color, uint8_t bgColor) {
	for (int i = 0; i < _fetchedEntryCount; ++i) {
		int spX = (_fetchedEntries[i]._x - 8) + (_lcd.scrollX % 8);
		if (spX + 8 < _pxQueue.queueX)
			continue ;
		int offset = _pxQueue.queueX - spX;
		if (offset < 0 || offset > 7)
			continue ;
		bit = (7 - offset);
		if (_fetchedEntries[i].h_flip)
			bit = offset;
		uint8_t hi = !!(_pxQueue.fetchEntryData[i * 2] & (1 << bit));
		uint8_t lo = !!(_pxQueue.fetchEntryData[(i * 2) + 1] & (1 << bit)) << 1;
		bool bgPrio = _fetchedEntries[i].dis_prio;
		if (!(hi|lo))
			continue;
		if (!bgPrio || bgColor == 0) {
			color = (_fetchedEntries[i].dis_prio) ?
				_lcd.sp2Color[hi|lo] : _lcd.sp1Color[hi|lo];
			if (hi|lo)
				break;
		}
	}
	return color;
}

void	Ppu::loadWinTile(){
	if (!isWinvisible())
		return;
	uint8_t winY = _lcd.winY;
	if (_pxQueue.fetchX + 7 >= _lcd.winX
		&& _pxQueue.fetchX + 7 <_lcd.winX + YRES + 14) {
		if (_lcd.ly >= winY && _lcd.ly < winY + XRES) {
			uint8_t wTileY = _winLine / 8;
			_pxQueue.bgwFetchData[0] = _bus.read(
				LCDC_WIN_MAP_AREA
				+ (_pxQueue.fetchX + 7 - _lcd.winX / 8)
				+ (wTileY * 32)
			);
			if (LCDC_BGW_DATA_AREA == 0x8800)
				_pxQueue.bgwFetchData[0] += 128;
		}
	}
}
void	Ppu::loadSptTile() {
	oamLineEntry *le = _lineSprites;
	while (le) {
		int spX = (le->entry._x - 8) + (_lcd.scrollX % 8);
		if ((spX >= _pxQueue.fetchX && spX < _pxQueue.fetchX + 8)
			|| ((spX + 8) >= _pxQueue.fetchX && (spX + 8) < _pxQueue.fetchX + 8)
		)
			_fetchedEntries[_fetchedEntryCount++] = le->entry;
		le = le->next;
		if (!le || _fetchedEntryCount >= 3)
			break;
	}
}
void	Ppu::loadSptData(uint8_t offset) {
	int curY = _lcd.ly;
	uint8_t	spriteHeight = LCDC_OBJ_HEIGHT;

	for (int i = 0; i < _fetchedEntryCount; ++i) {
		uint8_t ty = ((curY + 16) - _fetchedEntries[i]._y) * 2;
		if (_fetchedEntries[i].v_flip)
			ty = ((spriteHeight * 2) - 2) - ty;
		uint8_t tileIndex = _fetchedEntries[i]._tile;
		if (spriteHeight == 16)
			tileIndex &= ~(1);
		_pxQueue.fetchEntryData[(i * 2) + offset] = _bus.read(
			0x8000 + (tileIndex * 16) + ty + offset
		);
	}
}

void	Ppu::startDma(uint8_t start) {
	_dma.active = true;
	_dma.byte = 0;
	_dma.value = start;
	_dma.delay = 2;
}
void	Ppu::stepDma() {
	if (_dma.active)
		return ;
	if (_dma.delay) {
		--_dma.delay;
		return ;
	}
	writeOam(_dma.byte, _bus.read((_dma.value * 0x100) + _dma.byte));
	_dma.byte++;
	_dma.active = _dma.byte < 0xA0;
}
bool	Ppu::transferDma() {
	return _dma.active;
}
