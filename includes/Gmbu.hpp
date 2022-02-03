#ifndef _GBMU_HPP
#define _GBMU_HPP

#include <array>

#include "Cpu.hpp"
#include "Rom.hpp"
#include "Ram.hpp"
#include "utils.hpp"

class Gbmu
{
public:
	typedef struct {
		bool paused;
		bool running;
		uint64_t ticks;
	}	gbmu_context;

	gbmu_context	ctx;

	Rom				_rom;
	Cpu				_cpu;
	Ram				_ram;

public:
	Gbmu();
	~Gbmu();

	void write(uint16_t addr, uint8_t value);
	void write16(uint16_t addr, uint8_t value);
	uint8_t read(uint16_t addr);
	uint16_t read16(uint16_t addr);

	void stackPush(uint8_t data);
	void stackPush16(uint16_t data);
	uint8_t		stackPop();
	uint16_t	stackPop16();

	int	gbmu_run(int ac, char** av);
	gbmu_context* getContext();
};

#endif