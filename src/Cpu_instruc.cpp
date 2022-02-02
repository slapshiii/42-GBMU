#include "Cpu.hpp"
#include "Gmbu.hpp"

extern Gbmu gbmu;

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
static void gotoAddr(Cpu *c, uint16_t addr, bool pushpc) {
	if (checkCondition(c)) {
		if (pushpc) {
			c->_cycle += 2;
			gbmu.stackPush16(c->regs.pc);
		}
		c->regs.pc = addr;
		++c->_cycle;
	}
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
static bool is16Bit(reg_type rt) {
	return rt >= RT_AF;
}
static void proc_none(Cpu *c) {
	(void)c;
	dprintf(STDERR_FILENO, "Error: Invalid instruction: %02X\n", c->_opcode);
	exit(-4);
}
static void proc_nop(Cpu *c) {
	(void)c;
}
static void proc_stop(Cpu *c) {
	(void)c;
	dprintf(STDERR_FILENO, "Stop!\n");
}
static void proc_halt(Cpu *c) {
	c->_halted = true;
}
static void proc_ld(Cpu *c) {
	if (c->_destIsMem) {
		if (is16Bit(c->_cur_inst->reg2)) {
			++c->_cycle;
			gbmu.write16(c->_memDest, c->_fetchData);
		} else {
			gbmu.write(c->_memDest, c->_fetchData);
		}
		++c->_cycle;
		return;
	}
	if (c->_cur_inst->mode == AM_HL_SPR) {
		uint8_t hflags = (c->readReg(c->_cur_inst->reg2) & 0xF)
			+ (c->_fetchData & 0xF) >= 0x10;
		uint8_t cflags = (c->readReg(c->_cur_inst->reg2) & 0xFF)
			+ (c->_fetchData & 0xFF) >= 0x100;
		c->setFlags(0, 0, hflags, cflags);
		c->setReg(
			c->_cur_inst->reg1,
			c->readReg(c->_cur_inst->reg2) + (uint8_t)c->_fetchData
		);
		return;
	}
	c->setReg(c->_cur_inst->reg1, c->_fetchData);
}
static void proc_ldh(Cpu *c) {
	if (c->_cur_inst->reg1 == RT_A) {
		c->setReg(c->_cur_inst->reg1, gbmu.read(0xFF00 | c->_fetchData));
	} else {
		gbmu.write(c->_memDest, Cpu::regs.a);
	}
	++c->_cycle;
}
static void proc_jp(Cpu *c) {
	gotoAddr(c, c->_fetchData, false);
}
static void proc_jr(Cpu *c) {
	uint8_t rel = (uint8_t)(c->_fetchData & 0xFF);
	uint16_t addr = Cpu::regs.pc + rel;
	gotoAddr(c, addr, false);
}
static void proc_call(Cpu *c) {
	gotoAddr(c, c->_fetchData, true);
}
static void proc_rst(Cpu *c) {
	gotoAddr(c, c->_cur_inst->param, true);
}
static void proc_ret(Cpu *c) {
	if (c->_cur_inst->cond != CT_NONE)
		++c->_cycle;
	if (checkCondition(c)) {
		uint16_t lo = gbmu.stackPop();
		++c->_cycle;
		uint16_t hi = gbmu.stackPop();
		++c->_cycle;
		uint16_t n = (hi << 8) | lo;
		Cpu::regs.pc = n;
		++c->_cycle;
	}
}
static void proc_reti(Cpu *c) {
	c->_masterInt = true;
	proc_ret(c);
}
static void proc_di(Cpu *c) {
	c->_masterInt = false;
}
static void proc_ei(Cpu *c) {
	c->_enablingIme = true;
}
static void proc_pop(Cpu *c) {
	uint16_t lo = gbmu.stackPop();
	++c->_cycle;
	uint16_t hi = gbmu.stackPop();
	++c->_cycle;
	uint16_t n = (hi << 8) | lo;
	c->setReg(c->_cur_inst->reg1, n);
	if (c->_cur_inst->reg1 == RT_AF) {
		c->setReg(c->_cur_inst->reg1, n & 0xFFF0);
	}
}
static void proc_push(Cpu *c) {
	uint16_t hi = (c->readReg(c->_cur_inst->reg1) >> 8) & 0xFF;
	++c->_cycle;
	gbmu.stackPush(hi);
	uint16_t lo = c->readReg(c->_cur_inst->reg1) & 0xFF;
	++c->_cycle;
	gbmu.stackPush(lo);
	++c->_cycle;
}
static void proc_inc(Cpu *c) {
	uint16_t val = c->readReg(c->_cur_inst->reg1) + 1;
	if (is16Bit(c->_cur_inst->reg1))
		++c->_cycle;
	if (c->_cur_inst->reg1 == RT_HL && c->_cur_inst->mode == AM_MR) {
		val = gbmu.read(c->readReg(RT_HL)) + 1;
		val &= 0xFF;
		gbmu.write(c->readReg(RT_HL), val);
	} else {
		c->setReg(c->_cur_inst->reg1, val);
		val = c->readReg(c->_cur_inst->reg1);
	}
	if ((c->_opcode & 0x03) == 0x03)
		return;
	c->setFlags(val == 0, 0, (val & 0x0F) == 0, -1);
}
static void proc_dec(Cpu *c) {
	uint16_t val = c->readReg(c->_cur_inst->reg1) - 1;
	if (is16Bit(c->_cur_inst->reg1))
		++c->_cycle;
	if (c->_cur_inst->reg1 == RT_HL && c->_cur_inst->mode == AM_MR) {
		val = gbmu.read(c->readReg(RT_HL)) - 1;
		gbmu.write(c->readReg(RT_HL), val);
	} else {
		c->setReg(c->_cur_inst->reg1, val);
		val = c->readReg(c->_cur_inst->reg1);
	}
	if ((c->_opcode & 0x0B) == 0x0B)
		return;
	c->setFlags(val == 0, 1, (val & 0x0F) == 0, -1);
}
static void proc_daa(Cpu *c) {
	uint8_t	u = 0;
	int		fc = 0;
	if (CPU_FLAG_H || (!CPU_FLAG_N && (Cpu::regs.a & 0xF) > 9))
		u = 6;
	if (CPU_FLAG_C || (!CPU_FLAG_N && Cpu::regs.a > 0x99)) {
		u |= 0x60;
		fc = 1;
	}
	Cpu::regs.a += CPU_FLAG_N ? -u : u;
	c->setFlags(Cpu::regs.a == 0, -1, 0, fc);
}
static void proc_rra(Cpu *c) {
	uint8_t carry = CPU_FLAG_C;
	uint8_t newC = Cpu::regs.a & 1;
	Cpu::regs.a >>= 1;
	Cpu::regs.a |= (carry << 7);
	c->setFlags(0, 0, 0, newC);
}
static void proc_and(Cpu *c) {
	Cpu::regs.a &= c->_fetchData;
	c->setFlags(Cpu::regs.a == 0, 0, 1, 0);
}
static void proc_xor(Cpu *c) {
	Cpu::regs.a ^= c->_fetchData & 0xFF;
	c->setFlags(Cpu::regs.a == 0, 0, 0, 0);
}
static void proc_or(Cpu *c) {
	Cpu::regs.a |= c->_fetchData & 0xFF;
	c->setFlags(Cpu::regs.a == 0, 0, 0, 0);
}
static void proc_cp(Cpu *c) {
	int n = (int)Cpu::regs.a - (int)c->_fetchData;
	c->setFlags(n == 0, 1,
		((int)Cpu::regs.a & 0x0F) - ((int)c->_fetchData & 0x0F) < 0, n < 0
	);
}
static void proc_cpl(Cpu *c) {
	Cpu::regs.a = ~Cpu::regs.a;
	c->setFlags(-1, 1, 1, -1);
}
static void proc_scf(Cpu *c) {
	c->setFlags(-1, 0, 0, 1);
}
static void proc_ccf(Cpu *c) {
	c->setFlags(-1, 0, 0,CPU_FLAG_C ^ 1);
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
static void proc_sub(Cpu *cpu) {
	uint16_t val = cpu->readReg(cpu->_cur_inst->reg1) - cpu->_fetchData;
	int z = val == 0;
	int h = ((int)cpu->readReg(cpu->_cur_inst->reg1) & 0xF) - ((int)cpu->_fetchData & 0xF) < 0;
	int c = ((int)cpu->readReg(cpu->_cur_inst->reg1)) - ((int)cpu->_fetchData) < 0;
	cpu->setReg(cpu->_cur_inst->reg1, val);
	cpu->setFlags(z, 1, h, c);
}
static void proc_sbc(Cpu *cpu) {
	uint8_t val = cpu->_fetchData + CPU_FLAG_C;
	int z = cpu->readReg(cpu->_cur_inst->reg1) - val == 0;
	int h = ((int)cpu->readReg(cpu->_cur_inst->reg1) & 0xF)
		- ((int)cpu->_fetchData & 0xF) - ((int)CPU_FLAG_C) < 0;
	int c = ((int)cpu->readReg(cpu->_cur_inst->reg1))
		- ((int)cpu->_fetchData) - ((int)CPU_FLAG_C) < 0;
	cpu->setReg(cpu->_cur_inst->reg1, cpu->readReg(cpu->_cur_inst->reg1) - val);
	cpu->setFlags(z, 1, h, c);
}
static void proc_adc(Cpu *cpu) {
	uint16_t u = cpu->_fetchData;
	uint16_t a = Cpu::regs.a;
	uint16_t c = CPU_FLAG_C;
	Cpu::regs.a = (a + u + c) & 0xFF;
	cpu->setFlags(Cpu::regs.a == 0, 0,
		(a & 0xF) + (u & 0xF) + c > 0xF,
		a + u + c > 0xFF);
}
static void proc_add(Cpu *cpu) {
	uint32_t val = cpu->readReg(cpu->_cur_inst->reg1) + cpu->_fetchData;
	bool is16bit = is16Bit(cpu->_cur_inst->reg1);
	if (is16bit)
		++cpu->_cycle;
	if (cpu->_cur_inst->reg1 == RT_SP)
		val = cpu->readReg(cpu->_cur_inst->reg1) + (int8_t)cpu->_fetchData;
	int z = (val & 0xFF) == 0;
	int h = (cpu->readReg(cpu->_cur_inst->reg1) & 0xF) + (cpu->_fetchData & 0xF) >= 0x10;
	int c = (int)(cpu->readReg(cpu->_cur_inst->reg1) & 0xFF) + (int)(cpu->_fetchData & 0xFF) >= 0x100;
	if (is16bit) {
		z = -1;
		h = (cpu->readReg(cpu->_cur_inst->reg1) & 0xFFF) + (cpu->_fetchData & 0xFFF) >= 0x1000;
		uint32_t n = ((uint32_t)cpu->readReg(cpu->_cur_inst->reg1)) + ((uint32_t)cpu->_fetchData);
		c = n >= 0x10000;
	}
	if (cpu->_cur_inst->reg1 == RT_SP) {
		z = 0;
		h = (cpu->readReg(cpu->_cur_inst->reg1) & 0xF) + (cpu->_fetchData & 0xF) >= 0x10;
		c = (int)(cpu->readReg(cpu->_cur_inst->reg1) & 0xFF) + (int)(cpu->_fetchData & 0xFF) >= 0x100;
	}
	cpu->setReg(cpu->_cur_inst->reg1, val & 0xFFFF);
	cpu->setFlags(z, 0, h, c);
}

static IN_PROC processors[] = {
	[IN_NONE] = proc_none,
	[IN_NOP] = proc_nop,
	[IN_LD] = proc_ld,
	[IN_LDH] = proc_ldh,
	[IN_JP] = proc_jp,
	[IN_DI] = proc_di,
	[IN_POP] = proc_pop,
	[IN_PUSH] = proc_push,
	[IN_JR] = proc_jr,
	[IN_CALL] = proc_call,
	[IN_RET] = proc_ret,
	[IN_RST] = proc_rst,
	[IN_DEC] = proc_dec,
	[IN_INC] = proc_inc,
	[IN_ADD] = proc_add,
	[IN_ADC] = proc_adc,
	[IN_SUB] = proc_sub,
	[IN_SBC] = proc_sbc,
	[IN_AND] = proc_and,
	[IN_XOR] = proc_xor,
	[IN_OR] = proc_or,
	[IN_CP] = proc_cp,
	[IN_CB] = proc_cb,
	[IN_STOP] = proc_stop,
	[IN_HALT] = proc_halt,
	[IN_RRCA] = proc_rrca,
	[IN_RLCA] = proc_rlca,
	[IN_RRA] = proc_rra,
	[IN_RLA] = proc_rla,
	[IN_DAA] = proc_daa,
	[IN_CPL] = proc_cpl,
	[IN_SCF] = proc_scf,
	[IN_CCF] = proc_ccf,
	[IN_EI] = proc_ei,
	[IN_RETI] = proc_reti
};

IN_PROC instGetProcessor(in_type type) {
	return processors[type];
}
