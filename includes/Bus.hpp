#ifndef _BUS_HPP
#define _BUS_HPP

#include "utils.hpp"
#include "Rom.hpp"
#include "Cpu.hpp"
#include "Ppu.hpp"
#include "Timer.hpp"

class Ppu;
class Cpu;
class Timer;
class Rom;
class Bus
{
public:
	Bus(Rom &r, Cpu &c, Ppu &p, Timer &t);
	~Bus();

	uint8_t	read(uint16_t addr);
	void	write(uint16_t addr, uint8_t data);
	uint8_t	vram_read(uint16_t addr);
	void	vram_write(uint16_t addr, uint8_t data);

private:
	Rom		&_rom;
	Cpu		&_cpu;
	Ppu		&_ppu;
	Timer	&_timer;

	std::array<uint8_t, 0x2000>	_wram;
	std::array<uint8_t, 0x80>	_hram;
	std::array<uint8_t, 0x80>	_io;
};

#endif