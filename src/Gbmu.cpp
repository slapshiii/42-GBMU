#include "Gmbu.hpp"

Gbmu::Gbmu()
{
	for (uint8_t &i : ram) i = 0x00;
	cpu.connectGbmu(this);
}

Gbmu::~Gbmu()
{
}

void Gbmu::write(uint16_t addr, uint8_t data)
{
	if (addr >= 0x0000 && addr <= 0xFFFF)
		ram[addr] = data;
}

uint8_t Gbmu::read(uint16_t addr, bool rdOnly)
{
	if (addr >= 0x0000 && addr <= 0xFFFF)
		return ram[addr];
	return (0x00);
}