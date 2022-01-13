#ifndef _CPU_HPP
#define _CPU_HPP

#include "Gmbu.hpp"

#include <vector>

#define	HICODE(x) ((x & 0b00111000) >> 3)
#define	LOCODE(x) ((x & 0b00000111) >> 0)

class Gbmu;

class Cpu
{
private:
	Gbmu	*_gbmu = nullptr;
public:
	enum FLAGS
	{
		CY = (1 << 4),
		H = (1 << 5),
		N = (1 << 6),
		Z = (1 << 7)
	};
	uint8_t	A = 0x00;
	uint8_t	B = 0x00;
	uint8_t	C = 0x00;
	uint8_t	D = 0x00;
	uint8_t	E = 0x00;
	union F_REG
	{
		struct
		{
			uint8_t unused: 4;
			uint8_t CY: 4;
			uint8_t H: 4;
			uint8_t N: 4;
			uint8_t Z: 4;
		};
		
		uint8_t	F = 0x00;
	} flag;
	
	
	uint8_t		H = 0x00;
	uint8_t		L = 0x00;
	uint16_t	PC = 0x0000;
	uint16_t	SP = 0x0000;

	uint8_t		fetched_8bit = 0x00;
	uint16_t	fetched_16bit = 0x0000;

	uint16_t	addr_abs = 0x0000;
	uint8_t		opcode = 0x00;
	uint8_t		cycles = 0;

	struct INST
	{
		std::string name;
		uint8_t(Cpu::*operate)(void) = nullptr;
		uint8_t(Cpu::*srcmode)(void) = nullptr;
		uint8_t	cycles = 0;
	};
	std::vector<INST> lookup;

public:
	Cpu(/* args */);
	~Cpu();

	uint8_t		fetch();

	void clock();
	void reset();
	void irq();

	void connectGbmu(Gbmu *g) { _gbmu = g; }

private:
	void write(uint16_t addr, uint8_t data);
	uint8_t read(uint16_t addr);

private:
	uint8_t	IMP();
	uint8_t	IMM();
	uint8_t	IND();
	uint8_t	HLR();
	uint8_t	BCR();
	uint8_t	DER();
	uint8_t	CR();
	uint8_t	IDW();

	uint8_t	LDr();
	uint8_t	LDHL();
	uint8_t	LDA();
	uint8_t	LDs();
	uint8_t LDH();
	uint8_t	LDW();
	uint8_t	LDI();
	uint8_t	LDD();
	uint8_t	LDC();
	uint8_t	LDE();
	uint8_t LHI();
	uint8_t LHD();

	uint8_t	PUSH();
	uint8_t	POP();
	uint8_t	LDHL();
	uint8_t	ADD();
	uint8_t	ADC();
	uint8_t	SUB();
	uint8_t	SBC();
	uint8_t	AND();
	uint8_t	OR();
	uint8_t	XOR();
	uint8_t	CP();
	uint8_t	INC();
	uint8_t	DEC();

	uint8_t	RLCA();
	uint8_t	RLA();
	uint8_t	RRCA();
	uint8_t	RRA();
	uint8_t	RLC();
	uint8_t	RL();
	uint8_t	RRC();
	uint8_t	RR();
	uint8_t	SLA();
	uint8_t	SRA();
	uint8_t	SRL();
	uint8_t	SWAP();

	uint8_t	BIT();
	uint8_t	SET();
	uint8_t	RES();

	uint8_t	JP();
	uint8_t	JR();
	uint8_t	CALL();
	uint8_t	RET();
	uint8_t	RETI();
	uint8_t	RST();

	uint8_t	DAA();
	uint8_t	CPL();
	uint8_t	NOP();

	uint8_t	HALT();
	uint8_t	STOP();
};

#endif