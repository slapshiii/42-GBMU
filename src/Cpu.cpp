#include "Cpu.hpp"
#include "Gmbu.hpp"

extern Gbmu gbmu;

instruction instructions[0x100] = {
	[0x00] = {IN_NOP, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0},

	[0x05] = {IN_DEC, AM_R, RT_B, RT_NONE, CT_NONE, 0},

	[0x0E] = {IN_LD, AM_R_D8, RT_C, RT_NONE, CT_NONE, 0},
	
	//[0x31] = {IN_XOR, AM_R, RT_A, RT_NONE, CT_NONE, 0},
	[0xAF] = {IN_XOR, AM_R, RT_A, RT_NONE, CT_NONE, 0},

	[0xC3] = {IN_JP, AM_D16, RT_NONE, RT_NONE, CT_NONE, 0},
	[0xF3] = {IN_DI, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0},
};

Cpu::Cpu() {
	Cpu::regs.pc = 0x100;
	Cpu::regs.a = 0x01;
}
Cpu::~Cpu() {}

instruction *Cpu::instrucByOpcode(uint8_t byte) {
	return (&instructions[byte]);
}

void Cpu::fetch_instruction() {
	this->_opcode = gbmu.read(Cpu::regs.pc++);
	this->_cur_inst = instrucByOpcode(this->_opcode);
}

void Cpu::execute() {
	IN_PROC proc = instGetProcessor(this->_cur_inst->type);
	if (proc)
		proc(this);
}

bool Cpu::step(){
	if (!this->_halted) {
		uint16_t PC = Cpu::regs.pc;
		this->fetch_instruction();
		this->fetch_data();

		printf("%04X: (%02X %02X %02X)\n",
			PC, this->_opcode,
			gbmu.read(PC + 1), gbmu.read(PC + 2)
		);

		this->execute();
	}

	return (true);
}

uint16_t reverse(uint16_t n) {
	return ((n & 0xFF00) >> 8) | ((n & 0x00FF) << 8);
}

uint16_t Cpu::readReg(reg_type reg) {
	switch (reg)
	{
	case RT_A: return Cpu::regs.a;
	case RT_F: return Cpu::regs.f;
	case RT_B: return Cpu::regs.b;
	case RT_C: return Cpu::regs.c;
	case RT_D: return Cpu::regs.d;
	case RT_E: return Cpu::regs.e;
	case RT_H: return Cpu::regs.h;
	case RT_L: return Cpu::regs.l;

	case RT_AF: return reverse(*((uint16_t *)&Cpu::regs.a));
	case RT_BC: return reverse(*((uint16_t *)&Cpu::regs.b));
	case RT_DE: return reverse(*((uint16_t *)&Cpu::regs.d));
	case RT_HL: return reverse(*((uint16_t *)&Cpu::regs.h));
	
	case RT_PC: return Cpu::regs.pc;
	case RT_SP: return Cpu::regs.sp;

	default:	return 0;
	}
}

void Cpu::setReg(reg_type reg, uint16_t value) {
	switch (reg)
	{
	case RT_A: Cpu::regs.a = value & 0xFF; break;
	case RT_F: Cpu::regs.f = value & 0xFF; break;
	case RT_B: Cpu::regs.b = value & 0xFF; break;
	case RT_C: Cpu::regs.c = value & 0xFF; break;
	case RT_D: Cpu::regs.d = value & 0xFF; break;
	case RT_E: Cpu::regs.e = value & 0xFF; break;
	case RT_H: Cpu::regs.h = value & 0xFF; break;
	case RT_L: Cpu::regs.l = value & 0xFF; break;

	case RT_AF: *((uint16_t *)&Cpu::regs.a) = reverse(value); break;
	case RT_BC: *((uint16_t *)&Cpu::regs.b) = reverse(value); break;
	case RT_DE: *((uint16_t *)&Cpu::regs.d) = reverse(value); break;
	case RT_HL: *((uint16_t *)&Cpu::regs.h) = reverse(value); break;
	
	case RT_PC: Cpu::regs.pc = value; break;
	case RT_SP: Cpu::regs.sp = value; break;
	case RT_NONE:
	default:	return;
	}
}