#ifndef _LCD_HPP
#define _LCD_HPP

#include "utils.hpp"

#define	LCDC_BGW_ENABLE (BIT(this->_lcd.lcdc, 0))
#define	LCDC_OBJ_ENABLE (BIT(this->_lcd.lcdc, 1))
#define	LCDC_OBJ_HEIGHT (BIT(this->_lcd.lcdc, 2) ? 16 : 8)
#define	LCDC_BG_MAP_AREA (BIT(this->_lcd.lcdc, 3) ? 0x9C00 : 0x9800)
#define	LCDC_BGW_DATA_AREA (BIT(this->_lcd.lcdc, 4) ? 0x8000 : 0x8800)
#define	LCDC_WIN_ENABLE (BIT(this->_lcd.lcdc, 5))
#define	LCDC_WIN_MAP_AREA (BIT(this->_lcd.lcdc, 6) ? 0x9C00 : 0x9800)
#define	LCDC_LCD_ENABLE (BIT(this->_lcd.lcdc, 7))

#define LCDS_MODE ((lcd_mode)(this->_lcd.lcds & 0b11))
#define LCDS_MODE_SET(mode) { this->_lcd.lcds &= ~0b11; this->_lcd.lcds |= mode; }
#define LCDS_LYC (BIT(this->_lcd.lcds, 2))
#define LCDS_LYC_SET(b) (BITSET(this->_lcd.lcds, 2, b))

#define LCDS_STAT_INT(src) (this->_lcd.lcds & src)

typedef enum {
	MODE_HBLANK,
	MODE_VBLANK,
	MODE_OAM,
	MODE_XFER
}   lcdMode;

typedef enum {
	SS_HBLANK = (1 << 3),
	SS_VBLANK = (1 << 4),
	SS_OAM = (1 << 5),
	SS_LYC = (1 << 6)
} statSrc;

class Lcd
{
public:
	uint8_t lcdc;
	uint8_t lcds;
	uint8_t scrollY;
	uint8_t scrollX;
	uint8_t ly;
	uint8_t lyComp;
	uint8_t dma;
	uint8_t bgPal;
	uint8_t	objPal[2];
	uint8_t	winY;
	uint8_t	winX;

	uint32_t	bgColor[4];
	uint32_t	sp1Color[4];
	uint32_t	sp2Color[4];

public:
	Lcd(/* args */);
	~Lcd();

	uint8_t	read(uint16_t addr);
	void	updatePal(uint8_t value, uint8_t pal);
	void	write(uint16_t addr, uint8_t value);
};


#endif