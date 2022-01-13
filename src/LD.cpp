#include "Cpu.hpp"

uint8_t Cpu::LDr(){
	switch (HICODE(opcode))
	{
	case 0b111:
		A = fetched_8bit;
		break;
	case 0b000:
		B = fetched_8bit;
		break;
	case 0b001:
		C = fetched_8bit;
		break;
	case 0b010:
		D = fetched_8bit;
		break;
	case 0b011:
		E = fetched_8bit;
		break;
	case 0b100:
		H = fetched_8bit;
		break;
	case 0b101:
		L = fetched_8bit;
		break;
	default:
		break;
	}
	return (0);
}

uint8_t	Cpu::LDHL(){
	addr_abs = (H << 4) | L;
	write(addr_abs, fetched_8bit);
	return (0);
}

uint8_t	Cpu::LDA(){
	A = fetched_8bit;
	return (0);
}

uint8_t	Cpu::LDs(){
	addr_abs = (0xFF00 | C);
	write(addr_abs, A);
	return (0);
}

uint8_t Cpu::LDH(){
	addr_abs = (0xFF00 | addr_abs);
	write(addr_abs, A);
}
uint8_t Cpu::LDW(){
	write(addr_abs, A);
}
uint8_t Cpu::LDI(){
	A = fetched_8bit;
	++addr_abs;
	H = uint8_t(addr_abs >> 4);
	L = uint8_t(addr_abs & 0x00FF);
}
uint8_t Cpu::LDD(){
	A = fetched_8bit;
	--addr_abs;
	H = uint8_t(addr_abs >> 4);
	L = uint8_t(addr_abs & 0x00FF);
}
uint8_t	Cpu::LDC(){
	addr_abs = (B << 4) | C;
	write(addr_abs, A);
}
uint8_t	Cpu::LDE(){
	addr_abs = (D << 4) | E;
	write(addr_abs, A);
}
uint8_t	Cpu::LHI(){
	addr_abs = (H << 4) | L;
	write(addr_abs, A);
	++addr_abs;
	H = uint8_t(addr_abs >> 4);
	L = uint8_t(addr_abs & 0x00FF);
}
uint8_t	Cpu::LHD(){
	addr_abs = (H << 4) | L;
	write(addr_abs, A);
	--addr_abs;
	H = uint8_t(addr_abs >> 4);
	L = uint8_t(addr_abs & 0x00FF);
}
