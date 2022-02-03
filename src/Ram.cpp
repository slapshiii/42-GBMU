#include "Ram.hpp"

void	Ram::hramWrite(uint16_t addr, uint8_t data) {
	addr -= 0xFF80;
	this->hram[addr] = data;
}
void	Ram::wramWrite(uint16_t addr, uint8_t data) {
	addr -= 0xC000;
	this->wram[addr] = data;
}
uint8_t	Ram::hramRead(uint16_t addr) {
	addr -= 0xFF80;
	return this->hram[addr];
}
uint8_t	Ram::wramRead(uint16_t addr) {
	addr -= 0xC000;
	return this->wram[addr];
}


Ram::Ram(/* args */)
{
}

Ram::~Ram()
{
}