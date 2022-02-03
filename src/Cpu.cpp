#include "Cpu.hpp"
#include "Gmbu.hpp"

extern Gbmu gbmu;

Cpu::Cpu() {
	Cpu::regs.pc = 0x100;
	Cpu::regs.a = 0x01;
}
Cpu::~Cpu() {}

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

		printf("%04X: %-7s (%02X %02X %02X)\n",
			PC, instName(this->_cur_inst->type),this->_opcode,
			gbmu.read(PC + 1), gbmu.read(PC + 2)
		);

		this->execute();
	} else {
		++this->_cycle;
		if (this->_intFlags)
			this->_halted = false;
	}
	if (this->_masterInt) {
		this->handleInt();
		this->_enablingIme = false;
	}
	if (this->_enablingIme) {
		this->_masterInt = true;
	}

	return (true);
}

void Cpu::handleInt() {
	if (this->checkInt(0x40, IT_VBLANK)) {
	} else if (this->checkInt(0x48, IT_LCD_STAT)) {
	} else if (this->checkInt(0x50, IT_TIMER)) {
	} else if (this->checkInt(0x58, IT_SERIAL)) {
	} else if (this->checkInt(0x60, IT_JOYPAD)) {
	}
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

uint8_t	Cpu::getIeReg() {
	return (this->_ie_register);
}
void	Cpu::setIeReg(uint8_t val) {
	this->_ie_register = val;
}
uint8_t	Cpu::getIntFlags() {
	return (this->_intFlags);
}
void	Cpu::setIntFlags(uint8_t val) {
	this->_intFlags = val;
}
