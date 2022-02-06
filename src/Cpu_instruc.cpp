#include "Cpu.hpp"

bool	Cpu::checkCondition(){
	bool z = CPU_FLAG_Z;
	bool c = CPU_FLAG_C;
	switch(_cur_inst->cond) {
		case CT_NONE: return true;
		case CT_C: return c;
		case CT_NC: return !c;
		case CT_Z: return z;
		case CT_NZ: return !z;
	}
	return false;
}
void Cpu::setFlags(int8_t z, int8_t n, int8_t h, int8_t c) {
	if (z != -1)
		BITSET(regs.f, 7, z);
	if (n != -1)
		BITSET(regs.f, 6, n);
	if (h != -1)
		BITSET(regs.f, 5, h);
	if (c != -1)
		BITSET(regs.f, 4, c);
}
void Cpu::gotoAddr(uint16_t addr, bool pushpc) {
	if (checkCondition()) {
		if (pushpc) {
			cycle(2);
			stackPush16(Cpu::regs.pc);
		}
		Cpu::regs.pc = addr;
		cycle(1);
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
void Cpu::proc_none(void) {
	dprintf(STDERR_FILENO, "Error: Invalid instruction: %02X\n", _opcode);
}
void Cpu::proc_nop(void) {
}
void Cpu::proc_stop(void) {
	dprintf(STDERR_FILENO, "Stop!\n");
}
void Cpu::proc_halt(void) {
	_halted = true;
}
void Cpu::proc_ld(void) {
	if (_destIsMem) {
		if (is16Bit(_cur_inst->reg2)) {
			cycle(1);
			write16(_memDest, _fetchData);
		} else {
			write(_memDest, _fetchData);
		}
		cycle(1);
		return;
	}
	if (_cur_inst->mode == AM_HL_SPR) {
		uint8_t hflag = (readReg(_cur_inst->reg2) & 0xF)
			+ (_fetchData & 0xF) >= 0x10;
		uint8_t cflag = (readReg(_cur_inst->reg2) & 0xFF)
			+ (_fetchData & 0xFF) >= 0x100;
		setFlags(0, 0, hflag, cflag);
		setReg(
			_cur_inst->reg1,
			readReg(_cur_inst->reg2) + (int8_t)_fetchData
		);
		return;
	}
	setReg(_cur_inst->reg1, _fetchData);
}
void Cpu::proc_ldh(void) {
	if (_cur_inst->reg1 == RT_A) {
		setReg(_cur_inst->reg1, read(0xFF00 | _fetchData));
	} else {
		write(_memDest, regs.a);
	}
	cycle(1);
}
void Cpu::proc_jp(void) {
	gotoAddr(_fetchData, false);
}
void Cpu::proc_jr(void) {
	int8_t rel = (int8_t)(_fetchData & 0xFF);
	uint16_t addr = regs.pc + rel;
	gotoAddr(addr, false);
}
void Cpu::proc_call(void) {
	gotoAddr(_fetchData, true);
}
void Cpu::proc_rst(void) {
	gotoAddr(_cur_inst->param, true);
}
void Cpu::proc_ret(void) {
	if (_cur_inst->cond != CT_NONE)
		cycle(1);
	if (checkCondition()) {
		uint16_t lo = stackPop();
		cycle(1);
		uint16_t hi = stackPop();
		cycle(1);
		uint16_t n = (hi << 8) | lo;
		regs.pc = n;
		cycle(1);
	}
}
void Cpu::proc_reti(void) {
	_masterInt = true;
	proc_ret();
}
void Cpu::proc_di(void) {
	_masterInt = false;
}
void Cpu::proc_ei(void) {
	_enablingIme = true;
}
void Cpu::proc_pop(void) {
	uint16_t lo = stackPop();
	cycle(1);
	uint16_t hi = stackPop();
	cycle(1);
	uint16_t n = (hi << 8) | lo;
	setReg(_cur_inst->reg1, n);
	if (_cur_inst->reg1 == RT_AF) {
		setReg(_cur_inst->reg1, n & 0xFFF0);
	}
}
void Cpu::proc_push(void) {
	uint16_t hi = (readReg(_cur_inst->reg1) >> 8) & 0xFF;
	cycle(1);
	stackPush(hi);
	uint16_t lo = readReg(_cur_inst->reg1) & 0xFF;
	cycle(1);
	stackPush(lo);
	cycle(1);
}
void Cpu::proc_inc(void) {
	uint16_t val = readReg(_cur_inst->reg1) + 1;
	if (is16Bit(_cur_inst->reg1))
		cycle(1);
	if (_cur_inst->reg1 == RT_HL && _cur_inst->mode == AM_MR) {
		val = read(readReg(RT_HL)) + 1;
		val &= 0xFF;
		write(readReg(RT_HL), val);
	} else {
		setReg(_cur_inst->reg1, val);
		val = readReg(_cur_inst->reg1);
	}
	if ((_opcode & 0x03) == 0x03)
		return;
	setFlags(val == 0, 0, (val & 0x0F) == 0, -1);
}
void Cpu::proc_dec(void) {
	uint16_t val = readReg(_cur_inst->reg1) - 1;
	if (is16Bit(_cur_inst->reg1))
		cycle(1);
	if (_cur_inst->reg1 == RT_HL && _cur_inst->mode == AM_MR) {
		val = read(readReg(RT_HL)) - 1;
		write(readReg(RT_HL), val);
	} else {
		setReg(_cur_inst->reg1, val);
		val = readReg(_cur_inst->reg1);
	}
	if ((_opcode & 0x0B) == 0x0B)
		return;
	setFlags(val == 0, 1, (val & 0x0F) == 0x0F, -1);
}
void Cpu::proc_daa(void) {
	uint8_t	u = 0;
	int		fc = 0;
	if (CPU_FLAG_H || (!CPU_FLAG_N && (regs.a & 0xF) > 9))
		u = 6;
	if (CPU_FLAG_C || (!CPU_FLAG_N && regs.a > 0x99)) {
		u |= 0x60;
		fc = 1;
	}
	regs.a += CPU_FLAG_N ? -u : u;
	setFlags(regs.a == 0, -1, 0, fc);
}
void Cpu::proc_rra(void) {
	uint8_t carry = CPU_FLAG_C;
	uint8_t newC = regs.a & 1;
	regs.a >>= 1;
	regs.a |= (carry << 7);
	setFlags(0, 0, 0, newC);
}
void Cpu::proc_and(void) {
	regs.a &= _fetchData;
	setFlags(regs.a == 0, 0, 1, 0);
}
void Cpu::proc_xor(void) {
	regs.a ^= _fetchData & 0xFF;
	setFlags(regs.a == 0, 0, 0, 0);
}
void Cpu::proc_or(void) {
	regs.a |= _fetchData & 0xFF;
	setFlags(regs.a == 0, 0, 0, 0);
}
void Cpu::proc_cp(void) {
	int n = (int)regs.a - (int)_fetchData;
	setFlags(n == 0, 1,
		((int)regs.a & 0x0F) - ((int)_fetchData & 0x0F) < 0, n < 0
	);
}
void Cpu::proc_cpl(void) {
	regs.a = ~regs.a;
	setFlags(-1, 1, 1, -1);
}
void Cpu::proc_scf(void) {
	setFlags(-1, 0, 0, 1);
}
void Cpu::proc_ccf(void) {
	setFlags(-1, 0, 0, CPU_FLAG_C ^ 1);
}
void Cpu::proc_cb(void) {
	uint8_t op = _fetchData;
	reg_type reg = decodeReg(op & 0b111);
	uint8_t bit = (op >> 3) & 0b111;
	uint8_t bit_op = (op >> 6) & 0b11;
	uint8_t reg_val = readReg8(reg);

	cycle(1);

	if (reg == RT_HL) {
		cycle(2);
	}
	switch (bit_op)
	{
	case 1: //BIT
		setFlags(!(reg_val & (1 << bit)), 0, 1, -1);
		return;
	case 2: //RES
		reg_val &= ~(1 << bit);
		setReg8(reg, reg_val);
		return;
	case 3: //SET
		reg_val |= (1 << bit);
		setReg8(reg, reg_val);
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
		setReg8(reg, res);
		setFlags(res == 0, false, false, setC);
	} return;
	case 1: { //RRC
		uint8_t old = reg_val;
		reg_val >>= 1;
		reg_val |= (old << 7);
		setReg8(reg, reg_val);
		setFlags(!reg_val, false, false, old & 1);
	} return;
	case 2: { //RL
		uint8_t old = reg_val;
		reg_val <<= 1;
		reg_val |= flagC;
		setReg8(reg, reg_val);
		setFlags(!reg_val, false, false, !!(old & 0x80));
	} return;
	case 3: { //RR
		uint8_t old = reg_val;
		reg_val >>= 1;
		reg_val |= (flagC << 7);
		setReg8(reg, reg_val);
		setFlags(!reg_val, false, false, old & 1);
	} return;
	case 4: { //SLA
		uint8_t old = reg_val;
		reg_val <<= 1;
		setReg8(reg, reg_val);
		setFlags(!reg_val, false, false, !!(old & 0x80));
	} return;
	case 5: { //SRA
		uint8_t u = static_cast<int8_t>(reg_val) >> 1;
		setReg8(reg, u);
		setFlags(!u, 0, 0, reg_val & 1);
	} return;
	case 6: { //SWAP
		reg_val = ((reg_val & 0xF0) >> 4) | ((reg_val & 0xF) << 4);
		setReg8(reg, reg_val);
		setFlags(reg_val == 0, false, false, false);
	} return;
	case 7: { //SRL
		uint8_t u = reg_val >> 1;
		setReg8(reg, u);
		setFlags(!u, 0, 0, reg_val & 1);
	} return;
	}
	dprintf(STDERR_FILENO, "Error: Invalid CB: %02X\n", _opcode);
	exit(-5);
}
void Cpu::proc_rlca(void) {
	uint8_t u = regs.a;
	bool c = (u >> 7) & 1;
	u = (u << 1) | c;
	regs.a = u;
	setFlags(0, 0, 0, c);
}
void Cpu::proc_rrca(void) {
	uint8_t b = regs.a & 1;
	regs.a >>= 1;
	regs.a |= (b << 7);
	setFlags(0, 0, 0, b);
}
void Cpu::proc_rla(void) {
	uint8_t u = regs.a;
	uint8_t cf = CPU_FLAG_C;
	uint8_t c = (u >> 7) & 1;
	regs.a = (u << 1) | cf;
	setFlags(0, 0, 0, c);
}
void Cpu::proc_sub(void) {
	uint16_t val = readReg(_cur_inst->reg1) - _fetchData;
	int z = val == 0;
	int h = ((int)readReg(_cur_inst->reg1) & 0xF) - ((int)_fetchData & 0xF) < 0;
	int c = ((int)readReg(_cur_inst->reg1)) - ((int)_fetchData) < 0;
	setReg(_cur_inst->reg1, val);
	setFlags(z, 1, h, c);
}
void Cpu::proc_sbc(void) {
	uint8_t val = _fetchData + CPU_FLAG_C;
	int z = readReg(_cur_inst->reg1) - val == 0;
	int h = ((int)readReg(_cur_inst->reg1) & 0xF)
		- ((int)_fetchData & 0xF) - ((int)CPU_FLAG_C) < 0;
	int c = ((int)readReg(_cur_inst->reg1))
		- ((int)_fetchData) - ((int)CPU_FLAG_C) < 0;
	setReg(_cur_inst->reg1, readReg(_cur_inst->reg1) - val);
	setFlags(z, 1, h, c);
}
void Cpu::proc_adc(void) {
	uint16_t u = _fetchData;
	uint16_t a = regs.a;
	uint16_t c = CPU_FLAG_C;
	regs.a = (a + u + c) & 0xFF;
	setFlags(regs.a == 0, 0,
		(a & 0xF) + (u & 0xF) + c > 0xF,
		a + u + c > 0xFF);
}
void Cpu::proc_add(void) {
	uint32_t val = readReg(_cur_inst->reg1) + _fetchData;
	bool is16bit = is16Bit(_cur_inst->reg1);
	if (is16bit)
		cycle(1);
	if (_cur_inst->reg1 == RT_SP)
		val = readReg(_cur_inst->reg1) + (int8_t)_fetchData;
	int z = (val & 0xFF) == 0;
	int h = (readReg(_cur_inst->reg1) & 0xF) + (_fetchData & 0xF) >= 0x10;
	int c = (int)(readReg(_cur_inst->reg1) & 0xFF) + (int)(_fetchData & 0xFF) >= 0x100;
	if (is16bit) {
		z = -1;
		h = (readReg(_cur_inst->reg1) & 0xFFF) + (_fetchData & 0xFFF) >= 0x1000;
		uint32_t n = ((uint32_t)readReg(_cur_inst->reg1)) + ((uint32_t)_fetchData);
		c = n >= 0x10000;
	}
	if (_cur_inst->reg1 == RT_SP) {
		z = 0;
		h = (readReg(_cur_inst->reg1) & 0xF) + (_fetchData & 0xF) >= 0x10;
		c = (int)(readReg(_cur_inst->reg1) & 0xFF) + (int)(_fetchData & 0xFF) >= 0x100;
	}
	setReg(_cur_inst->reg1, val & 0xFFFF);
	setFlags(z, 0, h, c);
}
