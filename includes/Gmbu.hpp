#ifndef _GBMU_HPP
#define _GBMU_HPP

#include <array>
#include "Cpu.hpp"

class Gbmu
{
public:
	Gbmu();
	~Gbmu();

	void write(uint16_t addr, uint8_t data);
	uint8_t read(uint16_t addr, bool rdOnly);

public:
	Cpu cpu;
	std::array<uint8_t, 32 * 1024> ram;
};

#endif