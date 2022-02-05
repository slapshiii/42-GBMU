#ifndef _TIMER_HPP
#define _TIMER_HPP

#include "utils.hpp"

class Timer
{
public:

	uint16_t	div;
	uint8_t		tima;
	uint8_t		tma;
	uint8_t		tac;

public:
	Timer(/* args */);
	~Timer();

	bool step();
	void	write(uint16_t addr, uint8_t data);
	uint8_t read(uint16_t addr);
};

#endif