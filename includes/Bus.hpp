#ifndef _BUS_HPP
#define _BUS_HPP

#include "utils.hpp"
#include "Cpu.hpp"
#include "Ppu.hpp"
#include "Timer.hpp"

class Bus
{
public:
	Bus(Cpu &c, Ppu &p, Timer &t);
	~Bus();

	uint8_t	read(uint16_t addr);
	void	write(uint16_t addr, uint8_t data);
	uint8_t	vram_read(uint16_t addr);
	void	vram_write(uint16_t addr, uint8_t data);

private:
	Cpu		&_cpu;
	Ppu		&_ppu;
	Timer	&_timer;
};

#endif