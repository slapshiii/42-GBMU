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
void Cpu::cpuSetFlags(char z, char n, char h, char c) {
	if (z != -1)
		BITSET(Cpu::regs.f, 7, z);
	if (n != -1)
		BITSET(Cpu::regs.f, 6, n);
	if (h != -1)
		BITSET(Cpu::regs.f, 5, h);
	if (c != -1)
		BITSET(Cpu::regs.f, 4, c);
}

static void proc_none(Cpu *c) {
	(void)c;
	dprintf(STDERR_FILENO, "Error: Invalid instruction\n");
	exit(-4);
}
static void proc_nop(Cpu *c) {
	(void)c;
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
	Cpu::setFlags(Cpu::regs.a == 0, 0, 0, 0);
}

static IN_PROC processors[] = {
	[IN_NONE] = proc_none,
	[IN_NOP] = proc_nop,
	[IN_LD] = proc_ld,
	[IN_JP] = proc_jp,
	[IN_DI] = proc_di,
	[IN_XOR] = proc_xor,
};

IN_PROC instGetProcessor(in_type type) {
	return processors[type];
}
