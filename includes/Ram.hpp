#ifndef _RAM_HPP
#define _RAM_HPP

#include "utils.hpp"

class Ram
{
private:
	uint8_t	wram[0x2000];
	uint8_t hram[0x80];
public:
	Ram(/* args */);
	~Ram();

	void	hramWrite(uint16_t addr, uint8_t data);
	void	wramWrite(uint16_t addr, uint8_t data);
	uint8_t	hramRead(uint16_t addr);
	uint8_t	wramRead(uint16_t addr);
;};



#endif