#include "Cpu.hpp"

Cpu::Cpu(/* args */)
{
	using c = Cpu;
	lookup =
	{	/*			x0								x1								x2								x3								x4								x5								x6								x7								x8								x9								xA								xB								xC								xD								xE								xF				*/
/*0x*/	{ "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "LD" , &c::LDC, &c::IMP, 2 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "LD" , &c::LDr, &c::IMM, 2 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "LD" , &c::LDA, &c::BCR, 2 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "LD" , &c::LDr, &c::IMM, 2 }, { "NOP", &c::NOP, &c::IMP, 4 },
/*1x*/	{ "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "LD" , &c::LDE, &c::IMP, 2 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "LD" , &c::LDr, &c::IMM, 2 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "LD" , &c::LDA, &c::DER, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "LD" , &c::LDr, &c::IMM, 2 }, { "NOP", &c::NOP, &c::IMP, 4 },
/*2x*/	{ "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "LD" , &c::LHI, &c::IMP, 2 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "LD" , &c::LDr, &c::IMM, 2 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "LD" , &c::LDI, &c::HLR, 2 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "LD" , &c::LDr, &c::IMM, 2 }, { "NOP", &c::NOP, &c::IMP, 4 },
/*3x*/	{ "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "LD" , &c::LHD, &c::IMP, 2 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "LD", &c::LDHL, &c::IMM, 3 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "LD" , &c::LDD, &c::HLR, 2 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "LD" , &c::LDr, &c::IMM, 2 }, { "NOP", &c::NOP, &c::IMP, 4 },
/*4x*/	{ "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::HLR, 2 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::HLR, 2 }, { "LD" , &c::LDr, &c::IMP, 1 },
/*5x*/	{ "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::HLR, 2 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::HLR, 2 }, { "LD" , &c::LDr, &c::IMP, 1 },
/*6x*/	{ "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::HLR, 2 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::HLR, 2 }, { "LD" , &c::LDr, &c::IMP, 1 },
/*7x*/	{ "LD", &c::LDHL, &c::IMP, 2 }, { "LD", &c::LDHL, &c::IMP, 2 }, { "LD", &c::LDHL, &c::IMP, 2 }, { "LD", &c::LDHL, &c::IMP, 2 }, { "LD", &c::LDHL, &c::IMP, 2 }, { "LD", &c::LDHL, &c::IMP, 2 }, {"HALT", &c::HALT, &c::IMP, 1}, { "LD", &c::LDHL, &c::IMP, 2 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::IMP, 1 }, { "LD" , &c::LDr, &c::HLR, 2 }, { "LD" , &c::LDr, &c::IMP, 1 },
/*8x*/	{ "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 },
/*9x*/	{ "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 },
/*Ax*/	{ "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 },
/*Bx*/	{ "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 },
/*Cx*/	{ "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 },
/*Dx*/	{ "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 },
/*Ex*/	{ "LDH", &c::LDH, &c::IND, 3 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "LD" , &c::LDs, &c::IMP, 2 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "LD ", &c::LDW, &c::IDW, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 },
/*Fx*/	{ "LDH", &c::LDA, &c::IND, 3 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "LD" , &c::LDA, &c::CR , 2 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "LD ", &c::LDA, &c::IDW, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }, { "NOP", &c::NOP, &c::IMP, 4 }
	};
}

Cpu::~Cpu()
{
}

void Cpu::write(uint16_t addr, uint8_t data)
{
	_gbmu->write(addr, data);
}

uint8_t Cpu::read(uint16_t addr)
{
	return (_gbmu->read(addr, false));
}

void	Cpu::clock()
{
	uint8_t fetchCycle, operateCycle;
	if (cycles == 0)
	{
		opcode = read(PC);
		++PC;

		cycles = lookup[opcode].cycles;
		fetchCycle = (this->*lookup[opcode].srcmode)();
		operateCycle = (this->*lookup[opcode].operate)();
		cycles += (fetchCycle & operateCycle);
	}
}

uint8_t	Cpu::IMP(){
	switch (LOCODE(opcode))
	{
	case 0b111:
		fetched_8bit = A;
		break;
	case 0b000:
		fetched_8bit = B;
		break;
	case 0b001:
		fetched_8bit = C;
		break;
	case 0b010:
		fetched_8bit = D;
		break;
	case 0b011:
		fetched_8bit = E;
		break;
	case 0b100:
		fetched_8bit = H;
		break;
	case 0b101:
		fetched_8bit = L;
		break;
	default:
		break;
	}
	return (0);
}
uint8_t Cpu::IMM(){
	addr_abs = PC++;
	fetched_8bit = read(addr_abs);
	return (0);
}
uint8_t Cpu::IND(){
	addr_abs = PC++;
	addr_abs = (read(addr_abs) & 0x00FF);
	fetched_8bit = read(addr_abs);
	return (0);
}
uint8_t Cpu::IDW(){
	addr_abs = (read(PC++) << 4);
	addr_abs |= read(PC++);
	fetched_8bit = read(addr_abs);
	return (0);
}
uint8_t Cpu::HLR(){
	addr_abs = (H << 4) | L;
	fetched_8bit = read(addr_abs);
	return (0);
}
uint8_t Cpu::BCR(){
	addr_abs = (B << 4) | C;
	fetched_8bit = read(addr_abs);
	return (0);
}
uint8_t Cpu::DER(){
	addr_abs = (D << 4) | E;
	fetched_8bit = read(addr_abs);
	return (0);
}
uint8_t Cpu::CR(){
	addr_abs = (0xFF00 | C);
	fetched_8bit = read(addr_abs);
	return (0);
}