#ifndef _CPU_HPP
#define _CPU_HPP

#include <vector>

#include "utils.hpp"

#define CPU_FLAG_Z BIT(regs.f, 7)
#define CPU_FLAG_N BIT(regs.f, 6)
#define CPU_FLAG_H BIT(regs.f, 5)
#define CPU_FLAG_C BIT(regs.f, 4)

class Cpu
{
public:
	typedef void (Cpu::*IN_PROC)();
	Cpu::IN_PROC instGetProcessor(in_type type);
	typedef struct
	{
		uint8_t		a;
		uint8_t		f;
		uint8_t		b;
		uint8_t		c;
		uint8_t		d;
		uint8_t		e;
		uint8_t		h;
		uint8_t		l;
		uint16_t	pc;
		uint16_t	sp;
	}	cpuRegs;
	
	cpuRegs			regs;
	uint16_t		_fetchData;
	uint16_t		_memDest;
	uint8_t			_opcode;
	uint8_t			_cycle;
	uint8_t			_ie_register;
	uint8_t			_intFlags;
	instruction*	_cur_inst;
	bool			_stepping;
	bool			_halted;
	bool			_destIsMem;
	bool			_masterInt;
	bool			_enablingIme;
	uint32_t	cycles;

private:
	

public:
	Cpu(std::function<uint8_t(uint16_t)> read,
		std::function<void(uint16_t, uint8_t)> write,
		std::function<void(size_t)> cycle
	);
	~Cpu();

	bool step();

	std::function<uint8_t(uint16_t)>		read;
	std::function<void(uint16_t, uint8_t)>	write;
	std::function<void(size_t)>				cycle;

	void fetch_instruction();
	void fetch_data();
	void execute();
	void initInt(uint16_t addr);
	bool checkInt(uint16_t addr, int_type it);
	void requestInt(int_type it);
	void handleInt();

	uint8_t	getIeReg();
	void	setIeReg(uint8_t val);
	uint8_t	getIntFlags();
	void	setIntFlags(uint8_t val);
	
private:
	void	printDebug(uint16_t PC);
	void	printDebug(void);


	uint16_t	read16(uint16_t addr);
	void		write16(uint16_t addr, uint16_t value);
	void		stackPush(uint8_t data);
	void		stackPush16(uint16_t data);
	uint8_t		stackPop();
	uint16_t	stackPop16();

	bool	checkCondition();
	void	gotoAddr(uint16_t addr, bool pushpc);

	uint16_t	readReg(reg_type reg);
	void 		setReg(reg_type reg, uint16_t value);
	uint8_t		readReg8(reg_type reg);
	void		setReg8(reg_type reg, uint8_t value);
	void		setFlags(int8_t z, int8_t n, int8_t h, int8_t c);

	void	proc_none();
	void	proc_nop();
	void	proc_ld();
	void	proc_ldh();
	void	proc_jp();
	void	proc_di();
	void	proc_pop();
	void	proc_push();
	void	proc_jr();
	void	proc_call();
	void	proc_ret();
	void	proc_rst();
	void	proc_dec();
	void	proc_inc();
	void	proc_add();
	void	proc_adc();
	void	proc_sub();
	void	proc_sbc();
	void	proc_and();
	void	proc_xor();
	void	proc_or();
	void	proc_cp();
	void	proc_cb();
	void	proc_stop();
	void	proc_halt();
	void	proc_rrca();
	void	proc_rlca();
	void	proc_rra();
	void	proc_rla();
	void	proc_daa();
	void	proc_cpl();
	void	proc_scf();
	void	proc_ccf();
	void	proc_ei();
	void	proc_reti();

	const char *instName(in_type type);
	const char *rtName(reg_type type);
	instruction *instrucByOpcode(uint8_t byte);
};

#endif