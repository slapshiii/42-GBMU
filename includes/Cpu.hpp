#ifndef _CPU_HPP
#define _CPU_HPP

#include <vector>

#include "utils.hpp"

#define CPU_FLAG_Z BIT(Cpu::regs.f, 7)
#define CPU_FLAG_N BIT(Cpu::regs.f, 6)
#define CPU_FLAG_H BIT(Cpu::regs.f, 5)
#define CPU_FLAG_C BIT(Cpu::regs.f, 4)

class Cpu;

typedef enum {
	IN_NONE,
	IN_NOP,
	IN_LD,
	IN_INC,
	IN_DEC,
	IN_RLCA,
	IN_ADD,
	IN_RRCA,
	IN_STOP,
	IN_RLA,
	IN_JR,
	IN_RRA,
	IN_DAA,
	IN_CPL,
	IN_SCF,
	IN_CCF,
	IN_HALT,
	IN_ADC,
	IN_SUB,
	IN_SBC,
	IN_AND,
	IN_XOR,
	IN_OR,
	IN_CP,
	IN_POP,
	IN_JP,
	IN_PUSH,
	IN_RET,
	IN_CB,
	IN_CALL,
	IN_RETI,
	IN_LDH,
	IN_JPHL,
	IN_DI,
	IN_EI,
	IN_RST,
	IN_ERR,
	IN_RLC,
	IN_RRC,
	IN_RL,
	IN_RR,
	IN_SLA,
	IN_SRA,
	IN_SWAP,
	IN_SRL,
	IN_BIT,
	IN_RES,
	IN_SET
}	in_type;

typedef enum {
	AM_IMP,
	AM_R_D16, AM_R_R, AM_MR_R, AM_R, AM_R_D8, AM_R_MR,
	AM_R_HLI, AM_R_HLD, AM_HLI_R, AM_HLD_R, AM_R_A8, AM_A8_R, AM_HL_SPR,
	AM_D16, AM_D8, AM_D16_R, AM_MR_D8, AM_MR, AM_A16_R, AM_R_A16
}	addr_mode;

typedef enum {
	RT_NONE,
	RT_A, RT_F, RT_B, RT_C, RT_D, RT_E, RT_H, RT_L,
	RT_AF, RT_BC, RT_DE, RT_HL, RT_SP, RT_PC
}	reg_type;

typedef enum {
	CT_NONE, CT_NZ, CT_Z, CT_NC, CT_C
}	cond_type;

typedef struct {
	in_type		type;
	addr_mode	mode = AM_IMP;
	reg_type	reg1 = RT_NONE;
	reg_type	reg2 = RT_NONE;
	cond_type	cond = CT_NONE;
	uint8_t		param = 0;
}	instruction;

typedef enum {
	IT_VBLANK = 1,
	IT_LCD_STAT = 2,
	IT_TIMER = 4,
	IT_SERIAL = 8,
	IT_JOYPAD = 16
}	int_type;

typedef void (*IN_PROC)(Cpu *c);
IN_PROC instGetProcessor(in_type type);
const char *instName(in_type type);

class Gbmu;
class Cpu
{

public:
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
	
	static cpuRegs	regs;
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

	instruction *instrucByOpcode(uint8_t byte);

	void	(*writeBus)(uint16_t addr, uint8_t value);
	uint8_t	(*readBus)(uint16_t addr);

public:
	Cpu(std::function<uint8_t(uint16_t)> read,
		std::function<void(uint16_t, uint8_t)> write,
		std::function<void(size_t)> cycle
	);
	~Cpu();

	bool step();

	void fetch_instruction();
	void fetch_data();
	void execute();
	void initInt(uint16_t addr);
	bool checkInt(uint16_t addr, int_type it);
	void requestInt(int_type it);
	void handleInt();

	uint16_t readReg(reg_type reg);
	void setReg(reg_type reg, uint16_t value);
	void setFlags(int8_t z, int8_t n, int8_t h, int8_t c);

	uint8_t	getIeReg();
	void	setIeReg(uint8_t val);
	uint8_t	getIntFlags();
	void	setIntFlags(uint8_t val);
	
private:
	void	printDebug(uint16_t PC);

};

#endif