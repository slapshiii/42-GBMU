#include "Timer.hpp"
#include "Gbmu.hpp"

Timer::Timer() { this->div = 0xABCC; }
Timer::~Timer(){}

bool Timer::step() {
	uint16_t prevDiv = this->div;
	bool update = false;

	++this->div;
	switch (this->tac & (0b11))
	{
	case 0b00:
		update = (prevDiv & (1 << 9)) && (!(this->div & (1 << 9)));
		break;
	case 0b01:
		update = (prevDiv & (1 << 3)) && (!(this->div & (1 << 3)));
		break;
	case 0b10:
		update = (prevDiv & (1 << 5)) && (!(this->div & (1 << 5)));
		break;
	case 0b11:
		update = (prevDiv & (1 << 7)) && (!(this->div & (1 << 7)));
		break;
	}
	if (update && this->tac & (1 << 2)) {
		++this->tima;
		if (this->tima == 0xFF) {
			this->tima = this->tma;
			return (true);
		}
	}
	return (false);
}
void Timer::write(uint16_t addr, uint8_t data) {
	switch (addr)
	{
	case 0xFF04:
		this->div = 0;
		break;
	case 0xFF05:
		this->tima = data;
		break;
	case 0xFF06:
		this->tma = data;
		break;
	case 0xFF07:
		this->tac = data;
		break;
	}
}
uint8_t Timer::read(uint16_t addr) {
	switch (addr)
	{
	case 0xFF04:
		return this->div >> 8;
	case 0xFF05:
		return this->tima;
	case 0xFF06:
		return this->tma;
	case 0xFF07:
		return this->tac;
	}
	return 0;
}