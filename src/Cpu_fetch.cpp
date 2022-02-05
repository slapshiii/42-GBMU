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
	case AM_R_R:
		this->_fetchData = readReg(this->_cur_inst->reg2);
		return;
	case AM_R_D8:
		this->_fetchData = read(regs.pc);
		
		cycle(1);
		++regs.pc;
		return;
	case AM_R_D16:
	case AM_D16: {
		uint16_t lo = read(regs.pc);
		cycle(1);
		uint16_t hi = read(regs.pc + 1);
		cycle(1);
		this->_fetchData = lo | (hi << 8);
		regs.pc += 2;
		return;
	}
	case AM_MR_R:
		this->_fetchData = readReg(this->_cur_inst->reg2);
		this->_memDest = readReg(this->_cur_inst->reg1);
		this->_destIsMem = true;
		if (this->_cur_inst->reg1 == RT_C) //end page 86, start page 87
			this->_memDest |= 0xFF00;
		return;
	case AM_R_MR: {
		uint16_t addr = readReg(this->_cur_inst->reg2);
		if (this->_cur_inst->reg1 == RT_C) //end page 86, start page 87
			addr |= 0xFF00;
		this->_fetchData = read(addr);
		cycle(1);
		setFlags(regs.a == 0, 0, 0, 0);
	} return;
	case AM_R_HLI:
		this->_fetchData = read(readReg(this->_cur_inst->reg2));
		cycle(1);
		setReg(RT_HL, readReg(RT_HL) + 1);
		return;
	case AM_R_HLD:
		this->_fetchData = read(readReg(this->_cur_inst->reg2));
		cycle(1);
		setReg(RT_HL, readReg(RT_HL) - 1);
		return;
	case AM_HLI_R:
		this->_fetchData = readReg(this->_cur_inst->reg2);
		this->_memDest = readReg(this->_cur_inst->reg1);
		this->_destIsMem = true;
		setReg(RT_HL, readReg(RT_HL) + 1);
		return;
	case AM_HLD_R:
		this->_fetchData = readReg(this->_cur_inst->reg2);
		this->_memDest = readReg(this->_cur_inst->reg1);
		this->_destIsMem = true;
		setReg(RT_HL, readReg(RT_HL) - 1);
		return;
	case AM_R_A8:
		this->_fetchData = read(regs.pc);
		cycle(1);
		++regs.pc;
		return;
	case AM_A8_R:
		this->_memDest = read(regs.pc) | 0xFF00;
		this->_destIsMem = true;
		cycle(1);
		++regs.pc;
		return;
	case AM_HL_SPR:
		this->_fetchData = read(regs.pc);
		cycle(1);
		++regs.pc;
		return;
	case AM_D8:
		this->_fetchData = read(regs.pc);
		cycle(1);
		++regs.pc;
		return;
	case AM_A16_R:
	case AM_D16_R: {
		uint16_t lo = read(regs.pc);
		cycle(1);
		uint16_t hi = read(regs.pc + 1);
		cycle(1);
		this->_memDest = lo | (hi << 8);
		this->_destIsMem = true;
		regs.pc += 2;
		this->_fetchData = readReg(this->_cur_inst->reg2);
	} return;

	case AM_MR_D8:
		this->_fetchData = read(regs.pc);
		cycle(1);
		++regs.pc;
		this->_memDest = readReg(this->_cur_inst->reg1);
		this->_destIsMem = true;
		return;
	case AM_MR:
		this->_memDest = readReg(this->_cur_inst->reg1);
		this->_destIsMem = true;
		this->_fetchData = read(readReg(this->_cur_inst->reg1));
		cycle(1);
		return;
	case AM_R_A16: {
		uint16_t lo = read(regs.pc);
		cycle(1);
		uint16_t hi = read(regs.pc + 1);
		cycle(1);
		uint16_t addr = lo | (hi << 8);
		regs.pc += 2;
		this->_fetchData = read(addr);
		cycle(1);
	} return;
	default:
		dprintf(STDERR_FILENO, "Unknown Addressing Mode: %02X\n", this->_cur_inst->mode);
		exit (-3);
	}
}