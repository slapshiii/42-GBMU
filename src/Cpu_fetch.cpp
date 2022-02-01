#include "Cpu.hpp"

void Cpu::fetch_data() {
	this->_memDest = 0;
	this->_destIsMem = false;
	switch (this->_cur_inst->mode)
	{
	case AM_IMP: return;
	case AM_R:
		this->_fetchData = readReg(this->_cur_inst->reg1);
		return;
	case AM_R:
		this->_fetchData = readReg(this->_cur_inst->reg2);
		return;
	case AM_R_D8:
		this->_fetchData = gbmu.read(Cpu::regs.pc);
		++this->_cycle;
		++Cpu::regs.pc;
		return;
	case AM_R_D16:
	case AM_D16: {
		uint16_t lo = gbmu.read(Cpu::regs.pc);
		++this->_cycle;
		uint16_t hi = gbmu.read(Cpu::regs.pc + 1);
		++this->_cycle;
		this->_fetchData = lo | (hi << 8);
		Cpu::regs.pc += 2;
		return;
	}
	case AM_MR_R:
		this->_fetchData = readReg(this->_cur_inst->reg2);
		this->_memDest = readReg(this->_cur_inst->reg1);
		this->_destIsMem = true;
		if (this->_cur_inst->reg1 == RT_C) //end page 86, start page 87
			this->_memDest |= 0xFF00;
		return;
	case AM_R_MR:
		uint16_t addr = readReg(this->_cur_inst->reg2);
		if (this->_cur_inst->reg1 == RT_C) //end page 86, start page 87
			addr |= 0xFF00;
		this->_fetchData = gbmu.read(addr);
		++this->_cycle;
		Cpu::setFlags(Cpu::regs.a == 0, 0, 0, 0);
		return;
	case AM_R_HLI:
		this->_fetchData = gbmu.read(readReg(this->_cur_inst->reg2));
		++this->_cycle;
		setReg(RT_HL, readReg(RT_HL) + 1)
		return;
	case AM_R_HLD:
		this->_fetchData = gbmu.read(readReg(this->_cur_inst->reg2));
		++this->_cycle;
		setReg(RT_HL, readReg(RT_HL) - 1)
		return;

	default:
		dprintf(STDERR_FILENO, "Unknown Addressing Mode: %02X\n", this->_cur_inst->mode);
		exit (-3);
	}
}