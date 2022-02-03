#include "Ppu.hpp"

extern Gbmu gbmu;

Ppu::Ppu(){
    this->_curFrame = 0;
    this->_lineTicks = 0;
    this->_videoBuf = (uint32_t *)malloc(YRES * XRES * (sizeof(32)));

    this->_pxQueue.lineX = 0;
    this->_pxQueue.pushedX = 0;
    this->_pxQueue.fetchX = 0;
    this->_pxQueue.pixelQueue.size = 0;
    this->_pxQueue.pixelQueue.head = this->_pxQueue.pixelQueue.tail = NULL;
    this->_pxQueue.curFetchState = FS_TILE;

    this->_lineSprites = 0;
    this->_fetchedEntryCount = 0;
    this->_winLine = 0;
}
Ppu::~Ppu(){
    free(this->_videoBuf);
}

void	Ppu::step() {
    ++this->_lineTicks;
    /*switch (LCDS_MODE)
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
        break;*/
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
        //MODE XFER TODO
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
        //LCDS_MODE_SET(MODE_HBLANK);
        //if (LCDS_STAT_INT(SS_HBLANK)) {
            gbmu._cpu.requestInt(IT_LCD_STAT);
        //}
    }
}
void	Ppu::mode_vblank() {
    if (this->_lineTicks >= TICKS_PER_FRAME) {
        //increment_ly();
        if (lcd.ly >= LINES_PER_FRAME) {
            LCDS_MODE_SET(MODE_OAM);
            lcd.ly = 0;
            this->_winLine = 0;
        }
        this->_lineTicks = 0;
    }
}
void	Ppu::mode_hblank();
