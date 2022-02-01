#ifndef _GBMU_HPP
#define _GBMU_HPP

#include <array>

#include "Cpu.hpp"
#include "Rom.hpp"
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

public:
	Gbmu();
	~Gbmu();

	void write(uint16_t addr, uint8_t value);
	uint8_t read(uint16_t addr);

	int	gbmu_run(int ac, char** av);
	gbmu_context* getContext();
};

#endif