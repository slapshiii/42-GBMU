#include "Cpu.hpp"

static bool checkCondition(Cpu *cpu) {
	bool z = CPU_FLAG_Z;
	bool c = CPU_FLAG_C;
	switch(cpu->_cur_inst->cond) {
		case CT_NONE: return true;
		case CT_C: return c;
		case CT_NC: return !c;
		case CT_Z: return z;
		case CT_NZ: return !z;
	}
	return false;
}
void Cpu::setFlags(char z, char n, char h, char c) {
	if (z != -1)
		BITSET(Cpu::regs.f, 7, z);
	if (n != -1)
		BITSET(Cpu::regs.f, 6, n);
	if (h != -1)
		BITSET(Cpu::regs.f, 5, h);
	if (c != -1)
		BITSET(Cpu::regs.f, 4, c);
}
reg_type rt_lookup[] = {
	RT_B,
	RT_C,
	RT_D,
	RT_E,
	RT_H,
	RT_L,
	RT_HL,
	RT_A
};
reg_type decodeReg(uint8_t reg) {
	if (reg > 0b111) {
		return RT_NONE;
	}
	return rt_lookup[reg];
}
static void proc_none(Cpu *c) {
	(void)c;
	dprintf(STDERR_FILENO, "Error: Invalid instruction: %02X\n", c->_opcode);
	exit(-4);
}
static void proc_nop(Cpu *c) {
	(void)c;
}
static void proc_halt(Cpu *c) {
	c->_halted = true;
}
static void proc_ld(Cpu *c) {
	(void)c; //TODO
}
static void proc_jp(Cpu *c) {
	if (checkCondition(c)) {
		c->regs.pc = c->_fetchData;
		++c->_cycle;
	}
}
static void proc_di(Cpu *c) {
	c->_masterInt = false;
}
static void proc_xor(Cpu *c) {
	Cpu::regs.a ^= c->_fetchData & 0xFF;
	c->setFlags(Cpu::regs.a == 0, 0, 0, 0);
}
static void proc_cb(Cpu *c) {
	uint8_t op = c->_fetchData;
	reg_type reg = decodeReg(op & 0b111);
	uint8_t bit = (op >> 3) & 0b111;
	uint8_t bit_op = (op >> 6) & 011;
	uint8_t reg_val = c->readReg(reg);

	++c->_cycle;

	if (reg == RT_HL) {
		c->_cycle += 2;
	}
	switch (bit_op)
	{
	case 1: //BIT
		c->setFlags(!(reg_val & (1 << bit)), 0, 1, -1);
		return;
	case 2: //RST
		reg_val &= ~(1 << bit);
		c->setReg(reg, reg_val);
		return;
	case 3: //SET
		reg_val |= (1 << bit);
		c->setReg(reg, reg_val);
		return;
	}
	bool flagC = CPU_FLAG_C;
	switch (bit)
	{
	case 0: { //RLC
		bool setC = false;
		uint8_t res = (reg_val << 1) & 0xFF;
		if ((reg_val & (1 << 7)) != 0) {
			res |= 1;
			setC = true;
		}
		c->setReg(reg, res);
		c->setFlags(res == 0, 0, 0, setC);
	} return;
	case 1: { //RRC
		uint8_t old = reg_val;
		reg_val >>= 1;
		reg_val |= (old << 7);
		c->setReg(reg, reg_val);
		c->setFlags(!reg_val, 0, 0, old & 1);
	} return;
	case 2: { //RL
		uint8_t old = reg_val;
		reg_val <<= 1;
		reg_val |= flagC;
		c->setReg(reg, reg_val);
		c->setFlags(!reg_val, 0, 0, !!(old & 0x80));
	} return;
	case 3: { //RR
		uint8_t old = reg_val;
		reg_val >>= 1;
		reg_val |= (flagC << 7);
		c->setReg(reg, reg_val);
		c->setFlags(!reg_val, 0, 0, old & 1);
	} return;
	case 4: { //SLA
		uint8_t old = reg_val;
		reg_val <<= 1;
		c->setReg(reg, reg_val);
		c->setFlags(!reg_val, 0, 0, !!(old & 0x80));
	} return;
	case 5: { //SRA
		uint8_t u = (uint8_t)reg_val >> 1;
		c->setReg(reg, u);
		c->setFlags(!u, 0, 0, reg_val & 1);
	} return;
	case 6: { //SWAP
		reg_val = ((reg_val & 0xF0) >> 4) | ((reg_val & 0xF) << 4);
		c->setReg(reg, reg_val);
		c->setFlags(reg_val == 0, 0, 0, 0);
	} return;
	case 7: { //SRL
		uint8_t u = reg_val >> 1;
		c->setReg(reg, u);
		c->setFlags(!u, 0, 0, reg_val & 1);
	} return;
	}
	dprintf(STDERR_FILENO, "Error: Invalid CB: %02X\n", c->_opcode);
	exit(-5);

}
static void proc_rlca(Cpu *cpu) {
	uint8_t u = Cpu::regs.a;
	bool c = (u >> 7) & 1;
	u = (u << 1) | c;
	Cpu::regs.a = u;
	cpu->setFlags(0, 0, 0, c);
}
static void proc_rrca(Cpu *cpu) {
	uint8_t b = Cpu::regs.a & 1;
	Cpu::regs.a >>= 1;
	Cpu::regs.a |= (b << 7);
	cpu->setFlags(0, 0, 0, b);
}
static void proc_rla(Cpu *cpu) {
	uint8_t u = Cpu::regs.a;
	uint8_t cf = CPU_FLAG_C;
	uint8_t c = (u >> 7) & 1;
	Cpu::regs.a = (u << 1) | cf;
	cpu->setFlags(0, 0, 0, c);
}

static IN_PROC processors[] = {
	[IN_NONE] = proc_none,
	[IN_NOP] = proc_nop,
	[IN_LD] = proc_ld,
	[IN_JP] = proc_jp,
	[IN_DI] = proc_di,
	[IN_XOR] = proc_xor,
	[IN_CB] = proc_cb,
	[IN_HALT] = proc_halt,
	[IN_RRCA] = proc_rrca,
	[IN_RLCA] = proc_rlca,
	[IN_RLA] = proc_rla
};

IN_PROC instGetProcessor(in_type type) {
	return processors[type];
}
