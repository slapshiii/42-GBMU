#include "Cpu.hpp"
#include "Gbmu.hpp"

extern Gbmu gbmu;

Cpu::Cpu(std::function<uint8_t(uint16_t)> rd,
		std::function<void(uint16_t, uint8_t)> wr,
		std::function<void(size_t)> cy
) : read{std::move(rd)}, write{std::move(wr)}, cycle{std::move(cy)} {
	regs.pc = 0x100;
	regs.sp = 0xFFFE;
	*((short *)&Cpu::regs.a) = 0xB001;
	*((short *)&Cpu::regs.b) = 0x1300;
	*((short *)&Cpu::regs.d) = 0xD800;
	*((short *)&Cpu::regs.h) = 0x4D01;
	this->_ie_register = 0;
	this->_intFlags = 0;
	this->_masterInt = false;
	this->_enablingIme = false;
	cycles = 0;
}
Cpu::~Cpu() {}

void Cpu::fetch_instruction() {
	this->_opcode = read(regs.pc++);
	this->_cur_inst = instrucByOpcode(this->_opcode);
}

void Cpu::execute() {
	IN_PROC processors[0xFF] = {
		[IN_NONE] =	&Cpu::proc_none,
		[IN_NOP] =	&Cpu::proc_nop,
		[IN_LD] =	&Cpu::proc_ld,
		[IN_LDH] =	&Cpu::proc_ldh,
		[IN_JP] =	&Cpu::proc_jp,
		[IN_DI] =	&Cpu::proc_di,
		[IN_POP] =	&Cpu::proc_pop,
		[IN_PUSH] =	&Cpu::proc_push,
		[IN_JR] =	&Cpu::proc_jr,
		[IN_CALL] =	&Cpu::proc_call,
		[IN_RET] =	&Cpu::proc_ret,
		[IN_RST] =	&Cpu::proc_rst,
		[IN_DEC] =	&Cpu::proc_dec,
		[IN_INC] =	&Cpu::proc_inc,
		[IN_ADD] =	&Cpu::proc_add,
		[IN_ADC] =	&Cpu::proc_adc,
		[IN_SUB] =	&Cpu::proc_sub,
		[IN_SBC] =	&Cpu::proc_sbc,
		[IN_AND] =	&Cpu::proc_and,
		[IN_XOR] =	&Cpu::proc_xor,
		[IN_OR] =	&Cpu::proc_or,
		[IN_CP] =	&Cpu::proc_cp,
		[IN_CB] =	&Cpu::proc_cb,
		[IN_STOP] =	&Cpu::proc_stop,
		[IN_HALT] =	&Cpu::proc_halt,
		[IN_RRCA] =	&Cpu::proc_rrca,
		[IN_RLCA] =	&Cpu::proc_rlca,
		[IN_RRA] =	&Cpu::proc_rra,
		[IN_RLA] =	&Cpu::proc_rla,
		[IN_DAA] =	&Cpu::proc_daa,
		[IN_CPL] =	&Cpu::proc_cpl,
		[IN_SCF] =	&Cpu::proc_scf,
		[IN_CCF] =	&Cpu::proc_ccf,
		[IN_EI] =	&Cpu::proc_ei,
		[IN_RETI] =	&Cpu::proc_reti
	};
	IN_PROC proc = processors[_cur_inst->type];
	if (proc)
		(this->*proc)();
}
/*
static char db_msg[1024] = {0};
static int	msg_size = 0;
*/
bool Cpu::step(){
	if (!this->_halted) {
		++cycles;
		//printDebug();
		this->fetch_instruction();
		cycle(1);
		this->fetch_data();

		printDebug(regs.pc);
/*		if (read(0xFF02) == 0x81) {
			char c = read(0xFF01);
			db_msg[msg_size++] = c;
			write(0xFF02, 0);
		}
		if (db_msg[0]) {
			dprintf(STDERR_FILENO, "DB: %s\n", db_msg);
		}
*/
		this->execute();
	} else {
		cycle(1);
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

void Cpu::initInt(uint16_t addr) {
	stackPush16(regs.pc);
	regs.pc = addr;
}

bool Cpu::checkInt(uint16_t addr, int_type it) {
	if (this->_intFlags & it && this->_ie_register & it) {
		this->initInt(addr);
		this->_intFlags &= ~it;
		this->_halted = false;
		this->_masterInt = false;
		return (true);
	}
	return (false);
}

void Cpu::handleInt() {
	if (this->checkInt(0x40, IT_VBLANK)) {
	} else if (this->checkInt(0x48, IT_LCD_STAT)) {
	} else if (this->checkInt(0x50, IT_TIMER)) {
	} else if (this->checkInt(0x58, IT_SERIAL)) {
	} else if (this->checkInt(0x60, IT_JOYPAD)) {
	}
}
void Cpu::requestInt(int_type it) {
	this->_intFlags |= it;
}

uint16_t reverse(uint16_t n) {
	return ((n & 0xFF00) >> 8) | ((n & 0x00FF) << 8);
}

uint16_t Cpu::readReg(reg_type reg) {
	switch (reg)
	{
	case RT_A: return regs.a;
	case RT_F: return regs.f;
	case RT_B: return regs.b;
	case RT_C: return regs.c;
	case RT_D: return regs.d;
	case RT_E: return regs.e;
	case RT_H: return regs.h;
	case RT_L: return regs.l;

	case RT_AF: return reverse(*((uint16_t *)&regs.a));
	case RT_BC: return reverse(*((uint16_t *)&regs.b));
	case RT_DE: return reverse(*((uint16_t *)&regs.d));
	case RT_HL: return reverse(*((uint16_t *)&regs.h));
	
	case RT_PC: return regs.pc;
	case RT_SP: return regs.sp;

	default:	return 0;
	}
}

void Cpu::setReg(reg_type reg, uint16_t value) {
	switch (reg)
	{
	case RT_A: regs.a = value & 0xFF; break;
	case RT_F: regs.f = value & 0xFF; break;
	case RT_B: regs.b = value & 0xFF; break;
	case RT_C: regs.c = value & 0xFF; break;
	case RT_D: regs.d = value & 0xFF; break;
	case RT_E: regs.e = value & 0xFF; break;
	case RT_H: regs.h = value & 0xFF; break;
	case RT_L: regs.l = value & 0xFF; break;

	case RT_AF: *((uint16_t *)&regs.a) = reverse(value); break;
	case RT_BC: *((uint16_t *)&regs.b) = reverse(value); break;
	case RT_DE: *((uint16_t *)&regs.d) = reverse(value); break;
	case RT_HL: *((uint16_t *)&regs.h) = reverse(value); break;
	
	case RT_PC: regs.pc = value; break;
	case RT_SP: regs.sp = value; break;
	case RT_NONE:
	default:	return;
	}
}

uint8_t Cpu::readReg8(reg_type reg) {
	switch (reg)
	{
	case RT_A: return regs.a;
	case RT_F: return regs.f;
	case RT_B: return regs.b;
	case RT_C: return regs.c;
	case RT_D: return regs.d;
	case RT_E: return regs.e;
	case RT_H: return regs.h;
	case RT_L: return regs.l;
	case RT_HL: return read(readReg(RT_HL));
	default:	return 0;
	}
}

void Cpu::setReg8(reg_type reg, uint8_t value) {
	switch (reg)
	{
	case RT_A: regs.a = value & 0xFF; break;
	case RT_F: regs.f = value & 0xFF; break;
	case RT_B: regs.b = value & 0xFF; break;
	case RT_C: regs.c = value & 0xFF; break;
	case RT_D: regs.d = value & 0xFF; break;
	case RT_E: regs.e = value & 0xFF; break;
	case RT_H: regs.h = value & 0xFF; break;
	case RT_L: regs.l = value & 0xFF; break;
	case RT_HL: write(readReg(RT_HL), value); break;
	default:	return ;
	}
}

uint8_t	Cpu::getIntFlags() {
	return (this->_intFlags);
}
void	Cpu::setIntFlags(uint8_t val) {
	this->_intFlags = val;
}
uint8_t	Cpu::getIeReg() {
	return (this->_ie_register);
}
void	Cpu::setIeReg(uint8_t val) {
	this->_ie_register = val;
}

uint16_t Cpu::read16(uint16_t addr) {
	uint16_t lo = read(addr);
	uint16_t hi = read(addr + 1);
	return ((hi << 8) | lo);
}
void Cpu::write16(uint16_t addr, uint16_t value) {
	write(addr + 1, (uint8_t)((value >> 8) & 0xFF));
	write(addr, (uint8_t)(value & 0xFF));
}

void Cpu::stackPush(uint8_t data) {
	regs.sp--;
	write(regs.sp, data);
}
void Cpu::stackPush16(uint16_t data) {
	stackPush((data >> 8) & 0xFF);
	stackPush(data & 0xFF);
}
uint8_t		Cpu::stackPop() {
	return (read(regs.sp++));
}
uint16_t	Cpu::stackPop16() {
	uint16_t lo = stackPop();
	uint16_t hi = stackPop();
	return ((hi << 8) | lo);
}
