#include "Lcd.hpp"
#include "Gmbu.hpp"

static unsigned long colorDefault[4] = {
	0xFFFFFFFF,
	0xFFAAAAAA,
	0xFF555555,
	0xFF000000
};

Lcd::Lcd()
{
	this->lcdc = 0x91;
	this->scrollX = 0;
	this->scrollY = 0;
	this->ly = 0;
	this->lyComp = 0;
	this->bgPal = 0xFC;
	this->objPal[0] = 0xFF;
	this->objPal[1] = 0xFF;
	this->winY = 0;
	this->winX = 0;

	for (int i = 0; i < 4; ++i){
		this->bgColor[i] = colorDefault[i];
		this->sp1Color[i] = colorDefault[i];
		this->sp2Color[i] = colorDefault[i];
	}
}

Lcd::~Lcd(){}

uint8_t	Lcd::read(uint16_t addr) {
	uint8_t	newAddr = (addr - 0xFF40);
	uint8_t	*ptr = (uint8_t *)this;
	return (ptr[newAddr]);
}
void	Lcd::updatePal(uint8_t value, uint8_t pal) {
	uint32_t	*pColor = this->bgColor;
	switch (pal)
	{
	case 1:
		pColor = this->sp1Color;
		break;
	case 2:
		pColor = this->sp2Color;
		break;
	}
	pColor[0] = colorDefault[value & 0b11];
	pColor[1] = colorDefault[(value >> 2) & 0b11];
	pColor[2] = colorDefault[(value >> 4) & 0b11];
	pColor[3] = colorDefault[(value >> 6) & 0b11];
}
void	Lcd::write(uint16_t addr, uint8_t value) {
	uint8_t	newAddr = (addr - 0xFF40);
	uint8_t	*ptr = (uint8_t *)this;
	ptr[newAddr] = value;
	if (newAddr == 6) {
		//dma_start(value);
	}
	if (addr == 0xFF47)
		updatePal(value, 0);
	else if (addr == 0xFF48)
		updatePal(value & 0b11111100, 1);
	else if (addr == 0xFF49)
		updatePal(value & 0b11111100, 2);
}