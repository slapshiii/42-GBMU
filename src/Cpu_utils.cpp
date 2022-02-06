#include "Cpu.hpp"

instruction instructions[0x100] = {
	//0x0X
	[0x00] = {IN_NOP, AM_IMP  , RT_NONE, RT_NONE, CT_NONE, 0},
	[0x01] = {IN_LD , AM_R_D16, RT_BC  , RT_NONE, CT_NONE, 0},
	[0x02] = {IN_LD , AM_MR_R , RT_BC  , RT_A   , CT_NONE, 0},
	[0x03] = {IN_INC, AM_R    , RT_BC  , RT_NONE, CT_NONE, 0},
	[0x04] = {IN_INC, AM_R    , RT_B   , RT_NONE, CT_NONE, 0},
	[0x05] = {IN_DEC, AM_R    , RT_B   , RT_NONE, CT_NONE, 0},
	[0x06] = {IN_LD , AM_R_D8 , RT_B   , RT_NONE, CT_NONE, 0},
	[0x07] = {IN_RLCA,AM_IMP  , RT_NONE, RT_NONE, CT_NONE, 0},
	[0x08] = {IN_LD , AM_A16_R, RT_NONE, RT_SP},
	[0x09] = {IN_ADD, AM_R_R  , RT_HL  , RT_BC},
	[0x0A] = {IN_LD , AM_R_MR , RT_A   , RT_BC},
	[0x0B] = {IN_DEC, AM_R    , RT_BC},
	[0x0C] = {IN_INC, AM_R    , RT_C},
	[0x0D] = {IN_DEC, AM_R    , RT_C},
	[0x0E] = {IN_LD , AM_R_D8 , RT_C, RT_NONE, CT_NONE, 0},
	[0x0F] = {IN_RRCA},
	//0x1X
	[0x10] = {IN_STOP},
	[0x11] = {IN_LD , AM_R_D16, RT_DE},
	[0x12] = {IN_LD , AM_MR_R , RT_DE  , RT_A},
	[0x13] = {IN_INC, AM_R    , RT_DE  , RT_NONE, CT_NONE, 0},
	[0x14] = {IN_INC, AM_R    , RT_D   , RT_NONE, CT_NONE, 0},
	[0x15] = {IN_DEC, AM_R    , RT_D   , RT_NONE, CT_NONE, 0},
	[0x16] = {IN_LD , AM_R_D8 , RT_D   , RT_NONE, CT_NONE, 0},
	[0x17] = {IN_RLA, AM_IMP  , RT_NONE, RT_NONE, CT_NONE, 0},
	[0x18] = {IN_JR , AM_D8},
	[0x19] = {IN_ADD, AM_R_R  , RT_HL  , RT_DE},
	[0x1A] = {IN_LD , AM_R_MR , RT_A   , RT_DE},
	[0x1B] = {IN_DEC, AM_R    , RT_DE},
	[0x1C] = {IN_INC, AM_R    , RT_E},
	[0x1D] = {IN_DEC, AM_R    , RT_E},
	[0x1E] = {IN_LD , AM_R_D8 , RT_E, RT_NONE, CT_NONE, 0},
	[0x1F] = {IN_RRA},
	//0x2X
	[0x20] = {IN_JR , AM_D8   , RT_NONE, RT_NONE, CT_NZ},
	[0x21] = {IN_LD , AM_R_D16, RT_HL},
	[0x22] = {IN_LD , AM_HLI_R, RT_HL  , RT_A},
	[0x23] = {IN_INC, AM_R    , RT_HL  , RT_NONE, CT_NONE, 0},
	[0x24] = {IN_INC, AM_R    , RT_H   , RT_NONE, CT_NONE, 0},
	[0x25] = {IN_DEC, AM_R    , RT_H   , RT_NONE, CT_NONE, 0},
	[0x26] = {IN_LD , AM_R_D8 , RT_H   , RT_NONE, CT_NONE, 0},
	[0x27] = {IN_DAA, AM_IMP  , RT_NONE, RT_NONE, CT_NONE, 0},
	[0x28] = {IN_JR , AM_D8   , RT_NONE, RT_NONE, CT_Z},
	[0x29] = {IN_ADD, AM_R_R  , RT_HL  , RT_HL},
	[0x2A] = {IN_LD , AM_R_HLI, RT_A   , RT_HL},
	[0x2B] = {IN_DEC, AM_R    , RT_HL},
	[0x2C] = {IN_INC, AM_R    , RT_L},
	[0x2D] = {IN_DEC, AM_R    , RT_L},
	[0x2E] = {IN_LD , AM_R_D8 , RT_L, RT_NONE, CT_NONE, 0},
	[0x2F] = {IN_CPL},
	//0x3X
	[0x30] = {IN_JR , AM_D8   , RT_NONE, RT_NONE, CT_NC},
	[0x31] = {IN_LD , AM_R_D16, RT_SP},
	[0x32] = {IN_LD , AM_HLD_R, RT_HL  , RT_A},//
	[0x33] = {IN_INC, AM_R    , RT_SP  , RT_NONE, CT_NONE, 0},
	[0x34] = {IN_INC, AM_MR   , RT_HL  , RT_NONE, CT_NONE, 0},
	[0x35] = {IN_DEC, AM_MR   , RT_HL  , RT_NONE, CT_NONE, 0},
	[0x36] = {IN_LD , AM_MR_D8, RT_HL  , RT_NONE, CT_NONE, 0},
	[0x37] = {IN_SCF},
	[0x38] = {IN_JR , AM_D8   , RT_NONE, RT_NONE, CT_C},
	[0x39] = {IN_ADD, AM_R_R  , RT_HL  , RT_SP},
	[0x3A] = {IN_LD , AM_R_HLD, RT_A   , RT_HL},
	[0x3B] = {IN_DEC, AM_R    , RT_SP},
	[0x3C] = {IN_INC, AM_R    , RT_A},
	[0x3D] = {IN_DEC, AM_R    , RT_A},
	[0x3E] = {IN_LD , AM_R_D8 , RT_A, RT_NONE, CT_NONE, 0},
	[0x3F] = {IN_CCF},
	//0x4X
	[0x40] = {IN_LD , AM_R_R  , RT_B   , RT_B},
	[0x41] = {IN_LD , AM_R_R  , RT_B   , RT_C},
	[0x42] = {IN_LD , AM_R_R  , RT_B   , RT_D},
	[0x43] = {IN_LD , AM_R_R  , RT_B   , RT_E},
	[0x44] = {IN_LD , AM_R_R  , RT_B   , RT_H},
	[0x45] = {IN_LD , AM_R_R  , RT_B   , RT_L},
	[0x46] = {IN_LD , AM_R_MR , RT_B   , RT_HL},
	[0x47] = {IN_LD , AM_R_R  , RT_B   , RT_A},
	[0x48] = {IN_LD , AM_R_R  , RT_C   , RT_B},
	[0x49] = {IN_LD , AM_R_R  , RT_C   , RT_C},
	[0x4A] = {IN_LD , AM_R_R  , RT_C   , RT_D},
	[0x4B] = {IN_LD , AM_R_R  , RT_C   , RT_E},
	[0x4C] = {IN_LD , AM_R_R  , RT_C   , RT_H},
	[0x4D] = {IN_LD , AM_R_R  , RT_C   , RT_L},
	[0x4E] = {IN_LD , AM_R_MR , RT_C   , RT_HL},
	[0x4F] = {IN_LD , AM_R_R  , RT_C   , RT_A},
	//0x5X
	[0x50] = {IN_LD , AM_R_R  , RT_D   , RT_B},
	[0x51] = {IN_LD , AM_R_R  , RT_D   , RT_C},
	[0x52] = {IN_LD , AM_R_R  , RT_D   , RT_D},
	[0x53] = {IN_LD , AM_R_R  , RT_D   , RT_E},
	[0x54] = {IN_LD , AM_R_R  , RT_D   , RT_H},
	[0x55] = {IN_LD , AM_R_R  , RT_D   , RT_L},
	[0x56] = {IN_LD , AM_R_MR , RT_D   , RT_HL},
	[0x57] = {IN_LD , AM_R_R  , RT_D   , RT_A},
	[0x58] = {IN_LD , AM_R_R  , RT_E   , RT_B},
	[0x59] = {IN_LD , AM_R_R  , RT_E   , RT_C},
	[0x5A] = {IN_LD , AM_R_R  , RT_E   , RT_D},
	[0x5B] = {IN_LD , AM_R_R  , RT_E   , RT_E},
	[0x5C] = {IN_LD , AM_R_R  , RT_E   , RT_H},
	[0x5D] = {IN_LD , AM_R_R  , RT_E   , RT_L},
	[0x5E] = {IN_LD , AM_R_MR , RT_E   , RT_HL},
	[0x5F] = {IN_LD , AM_R_R  , RT_E   , RT_A},
	//0x6X
	[0x60] = {IN_LD , AM_R_R  , RT_H   , RT_B},
	[0x61] = {IN_LD , AM_R_R  , RT_H   , RT_C},
	[0x62] = {IN_LD , AM_R_R  , RT_H   , RT_D},
	[0x63] = {IN_LD , AM_R_R  , RT_H   , RT_E},
	[0x64] = {IN_LD , AM_R_R  , RT_H   , RT_H},
	[0x65] = {IN_LD , AM_R_R  , RT_H   , RT_L},
	[0x66] = {IN_LD , AM_R_MR , RT_H   , RT_HL},
	[0x67] = {IN_LD , AM_R_R  , RT_H   , RT_A},
	[0x68] = {IN_LD , AM_R_R  , RT_L   , RT_B},
	[0x69] = {IN_LD , AM_R_R  , RT_L   , RT_C},
	[0x6A] = {IN_LD , AM_R_R  , RT_L   , RT_D},
	[0x6B] = {IN_LD , AM_R_R  , RT_L   , RT_E},
	[0x6C] = {IN_LD , AM_R_R  , RT_L   , RT_H},
	[0x6D] = {IN_LD , AM_R_R  , RT_L   , RT_L},
	[0x6E] = {IN_LD , AM_R_MR , RT_L   , RT_HL},
	[0x6F] = {IN_LD , AM_R_R  , RT_L   , RT_A},
	//0x7X
	[0x70] = {IN_LD , AM_MR_R , RT_HL  , RT_B},
	[0x71] = {IN_LD , AM_MR_R , RT_HL  , RT_C},
	[0x72] = {IN_LD , AM_MR_R , RT_HL  , RT_D},
	[0x73] = {IN_LD , AM_MR_R , RT_HL  , RT_E},
	[0x74] = {IN_LD , AM_MR_R , RT_HL  , RT_H},
	[0x75] = {IN_LD , AM_MR_R , RT_HL  , RT_L},
	[0x76] = {IN_HALT},
	[0x77] = {IN_LD , AM_MR_R , RT_HL  , RT_A},
	[0x78] = {IN_LD , AM_R_R  , RT_A   , RT_B},
	[0x79] = {IN_LD , AM_R_R  , RT_A   , RT_C},
	[0x7A] = {IN_LD , AM_R_R  , RT_A   , RT_D},
	[0x7B] = {IN_LD , AM_R_R  , RT_A   , RT_E},
	[0x7C] = {IN_LD , AM_R_R  , RT_A   , RT_H},
	[0x7D] = {IN_LD , AM_R_R  , RT_A   , RT_L},
	[0x7E] = {IN_LD , AM_R_MR , RT_A   , RT_HL},
	[0x7F] = {IN_LD , AM_R_R  , RT_A   , RT_A},
	//0x8X
	[0x80] = {IN_ADD, AM_R_R  , RT_A   , RT_B},
	[0x81] = {IN_ADD, AM_R_R  , RT_A   , RT_C},
	[0x82] = {IN_ADD, AM_R_R  , RT_A   , RT_D},
	[0x83] = {IN_ADD, AM_R_R  , RT_A   , RT_E},
	[0x84] = {IN_ADD, AM_R_R  , RT_A   , RT_H},
	[0x85] = {IN_ADD, AM_R_R  , RT_A   , RT_L},
	[0x86] = {IN_ADD, AM_R_MR , RT_A   , RT_HL},
	[0x87] = {IN_ADD, AM_R_R  , RT_A   , RT_A},
	[0x88] = {IN_ADC, AM_R_R  , RT_A   , RT_B},
	[0x89] = {IN_ADC, AM_R_R  , RT_A   , RT_C},
	[0x8A] = {IN_ADC, AM_R_R  , RT_A   , RT_D},
	[0x8B] = {IN_ADC, AM_R_R  , RT_A   , RT_E},
	[0x8C] = {IN_ADC, AM_R_R  , RT_A   , RT_H},
	[0x8D] = {IN_ADC, AM_R_R  , RT_A   , RT_L},
	[0x8E] = {IN_ADC, AM_R_MR , RT_A   , RT_HL},
	[0x8F] = {IN_ADC, AM_R_R  , RT_A   , RT_A},
	//0x9X
	[0x90] = {IN_SUB, AM_R_R  , RT_A   , RT_B},
	[0x91] = {IN_SUB, AM_R_R  , RT_A   , RT_C},
	[0x92] = {IN_SUB, AM_R_R  , RT_A   , RT_D},
	[0x93] = {IN_SUB, AM_R_R  , RT_A   , RT_E},
	[0x94] = {IN_SUB, AM_R_R  , RT_A   , RT_H},
	[0x95] = {IN_SUB, AM_R_R  , RT_A   , RT_L},
	[0x96] = {IN_SUB, AM_R_MR , RT_A   , RT_HL},
	[0x97] = {IN_SUB, AM_R_R  , RT_A   , RT_A},
	[0x98] = {IN_SBC, AM_R_R  , RT_A   , RT_B},
	[0x99] = {IN_SBC, AM_R_R  , RT_A   , RT_C},
	[0x9A] = {IN_SBC, AM_R_R  , RT_A   , RT_D},
	[0x9B] = {IN_SBC, AM_R_R  , RT_A   , RT_E},
	[0x9C] = {IN_SBC, AM_R_R  , RT_A   , RT_H},
	[0x9D] = {IN_SBC, AM_R_R  , RT_A   , RT_L},
	[0x9E] = {IN_SBC, AM_R_MR , RT_A   , RT_HL},
	[0x9F] = {IN_SBC, AM_R_R  , RT_A   , RT_A},
	//0xAX
	[0xA0] = {IN_AND, AM_R_R  , RT_A   , RT_B},
	[0xA1] = {IN_AND, AM_R_R  , RT_A   , RT_C},
	[0xA2] = {IN_AND, AM_R_R  , RT_A   , RT_D},
	[0xA3] = {IN_AND, AM_R_R  , RT_A   , RT_E},
	[0xA4] = {IN_AND, AM_R_R  , RT_A   , RT_H},
	[0xA5] = {IN_AND, AM_R_R  , RT_A   , RT_L},
	[0xA6] = {IN_AND, AM_R_MR , RT_A   , RT_HL},
	[0xA7] = {IN_AND, AM_R_R  , RT_A   , RT_A},
	[0xA8] = {IN_XOR, AM_R_R  , RT_A   , RT_B},
	[0xA9] = {IN_XOR, AM_R_R  , RT_A   , RT_C},
	[0xAA] = {IN_XOR, AM_R_R  , RT_A   , RT_D},
	[0xAB] = {IN_XOR, AM_R_R  , RT_A   , RT_E},
	[0xAC] = {IN_XOR, AM_R_R  , RT_A   , RT_H},
	[0xAD] = {IN_XOR, AM_R_R  , RT_A   , RT_L},
	[0xAE] = {IN_XOR, AM_R_MR , RT_A   , RT_HL},
	[0xAF] = {IN_XOR, AM_R_R  , RT_A   , RT_A},
	//0xBX
	[0xB0] = {IN_OR , AM_R_R  , RT_A   , RT_B},
	[0xB1] = {IN_OR , AM_R_R  , RT_A   , RT_C},
	[0xB2] = {IN_OR , AM_R_R  , RT_A   , RT_D},
	[0xB3] = {IN_OR , AM_R_R  , RT_A   , RT_E},
	[0xB4] = {IN_OR , AM_R_R  , RT_A   , RT_H},
	[0xB5] = {IN_OR , AM_R_R  , RT_A   , RT_L},
	[0xB6] = {IN_OR , AM_R_MR , RT_A   , RT_HL},
	[0xB7] = {IN_OR , AM_R_R  , RT_A   , RT_A},
	[0xB8] = {IN_CP , AM_R_R  , RT_A   , RT_B},
	[0xB9] = {IN_CP , AM_R_R  , RT_A   , RT_C},
	[0xBA] = {IN_CP , AM_R_R  , RT_A   , RT_D},
	[0xBB] = {IN_CP , AM_R_R  , RT_A   , RT_E},
	[0xBC] = {IN_CP , AM_R_R  , RT_A   , RT_H},
	[0xBD] = {IN_CP , AM_R_R  , RT_A   , RT_L},
	[0xBE] = {IN_CP , AM_R_MR , RT_A   , RT_HL},
	[0xBF] = {IN_CP , AM_R_R  , RT_A   , RT_A},
	//0xCX
	[0xC0] = {IN_RET, AM_IMP  , RT_NONE, RT_NONE, CT_NZ},
	[0xC1] = {IN_POP, AM_R    , RT_BC},
	[0xC2] = {IN_JP , AM_D16  , RT_NONE, RT_NONE, CT_NZ},
	[0xC3] = {IN_JP , AM_D16},
	[0xC4] = {IN_CALL,AM_D16  , RT_NONE, RT_NONE, CT_NZ},
	[0xC5] = {IN_PUSH,AM_R    , RT_BC},
	[0xC6] = {IN_ADD, AM_R_D8 , RT_A},
	[0xC7] = {IN_RST, AM_IMP  , RT_NONE, RT_NONE, CT_NONE, 0x00},
	[0xC8] = {IN_RET, AM_IMP  , RT_NONE, RT_NONE, CT_Z},
	[0xC9] = {IN_RET},
	[0xCA] = {IN_JP , AM_D16  , RT_NONE, RT_NONE, CT_Z},
	[0xCB] = {IN_CB , AM_D8},
	[0xCC] = {IN_CALL,AM_D16  , RT_NONE, RT_NONE, CT_Z},
	[0xCD] = {IN_CALL,AM_D16},
	[0xCE] = {IN_ADC, AM_R_D8, RT_A},
	[0xCF] = {IN_RST, AM_IMP  , RT_NONE, RT_NONE, CT_NONE, 0x08},
	//0xDX
	[0xD0] = {IN_RET, AM_IMP  , RT_NONE, RT_NONE, CT_NC},
	[0xD1] = {IN_POP, AM_R    , RT_DE},
	[0xD2] = {IN_JP , AM_D16  , RT_NONE, RT_NONE, CT_NC},
	//[0xD3],
	[0xD4] = {IN_CALL,AM_D16  , RT_NONE, RT_NONE, CT_NC},
	[0xD5] = {IN_PUSH,AM_R    , RT_DE},
	[0xD6] = {IN_SUB, AM_R_D8 , RT_A},
	[0xD7] = {IN_RST, AM_IMP  , RT_NONE, RT_NONE, CT_NONE, 0x10},
	[0xD8] = {IN_RET, AM_IMP  , RT_NONE, RT_NONE, CT_C},
	[0xD9] = {IN_RETI},
	[0xDA] = {IN_JP , AM_D16  , RT_NONE, RT_NONE, CT_C},
	 //[0xDB],
	[0xDC] = {IN_CALL,AM_D16  , RT_NONE, RT_NONE, CT_C},
	//[0xDD],
	[0xDE] = {IN_SBC, AM_R_D8, RT_A},
	[0xDF] = {IN_RST, AM_IMP  , RT_NONE, RT_NONE, CT_NONE, 0x18},
	//0xEX
	[0xE0] = {IN_LDH, AM_A8_R , RT_NONE, RT_A},
	[0xE1] = {IN_POP, AM_R    , RT_HL},
	[0xE2] = {IN_LD , AM_MR_R , RT_C   , RT_A},
	//[0xE3],
	//[0xE4],
	[0xE5] = {IN_PUSH,AM_R    , RT_HL},
	[0xE6] = {IN_AND, AM_R_D8 , RT_A},
	[0xE7] = {IN_RST, AM_IMP  , RT_NONE, RT_NONE, CT_NONE, 0x20},
	[0xE8] = {IN_ADD, AM_R_D8 , RT_SP},
	[0xE9] = {IN_JP , AM_R    , RT_HL},
	[0xEA] = {IN_LD , AM_A16_R, RT_NONE, RT_A},
	//[0xEB],
	//[0xEC],
	//[0xED],
	[0xEE] = {IN_XOR, AM_R_D8, RT_A},
	[0xEF] = {IN_RST, AM_IMP  , RT_NONE, RT_NONE, CT_NONE, 0x28},
	//0xFX
	[0xF0] = {IN_LDH, AM_R_A8 , RT_A},
	[0xF1] = {IN_POP, AM_R    , RT_AF},
	[0xF2] = {IN_LD , AM_R_MR , RT_A   , RT_C},
	[0xF3] = {IN_DI},
	//[0xF4],
	[0xF5] = {IN_PUSH,AM_R    , RT_AF},
	[0xF6] = {IN_OR , AM_R_D8 , RT_A},
	[0xF7] = {IN_RST, AM_IMP  , RT_NONE, RT_NONE, CT_NONE, 0x30},
	[0xF8] = {IN_LD ,AM_HL_SPR, RT_HL  , RT_SP},
	[0xF9] = {IN_LD , AM_R_R  , RT_SP  , RT_HL},
	[0xFA] = {IN_LD , AM_R_A16, RT_A},
	[0xFB] = {IN_EI},
	//[0xFC],
	//[0xFD],
	[0xFE] = {IN_CP , AM_R_D8, RT_A},
	[0xFF] = {IN_RST, AM_IMP , RT_NONE, RT_NONE, CT_NONE, 0x38},

};

instruction *Cpu::instrucByOpcode(uint8_t byte) {
	return (&instructions[byte]);
}

std::string rtLookup[] = {
	"<NONE>",
	"A",
	"F",
	"B",
	"C",
	"D",
	"E",
	"H",
	"L",
	"AF",
	"BC",
	"DE",
	"HL",
	"SP",
	"PC",
};

std::string instLookup[] = {
	"NONE",
	"NOP",
	"LD",
	"INC",
	"DEC",
	"RLCA",
	"ADD",
	"RRCA",
	"STOP",
	"RLA",
	"JR",
	"RRA",
	"DAA",
	"CPL",
	"SCF",
	"CCF",
	"HALT",
	"ADC",
	"SUB",
	"SBC",
	"AND",
	"XOR",
	"OR",
	"CP",
	"POP",
	"JP",
	"PUSH",
	"RET",
	"CB",
	"CALL",
	"RETI",
	"LDH",
	"JPHL",
	"DI",
	"EI",
	"RST",
	"ERR",
	"RLC",
	"RRC",
	"RL",
	"RR",
	"SLA",
	"SRA",
	"SWAP",
	"SRL",
	"BIT",
	"RES",
	"SET"
};

const char *Cpu::instName(in_type type) {
	return instLookup[type].c_str();
}
const char *Cpu::rtName(reg_type type) {
	return rtLookup[type].c_str();
}

void Cpu::printDebug(uint16_t PC) {
	char inst[16] = {0};
	char flags[16];
	sprintf(flags, "%c%c%c%c",
		CPU_FLAG_Z ? 'Z' : '-',
		CPU_FLAG_N ? 'N' : '-',
		CPU_FLAG_H ? 'H' : '-',
		CPU_FLAG_C ? 'C' : '-'
	);
	sprintf(inst, "%-4s ", instName(_cur_inst->type));
	switch (this->_cur_inst->mode)
	{
	case AM_IMP:
		sprintf(inst, "%s ", instName(_cur_inst->type));
		break;
	case AM_R_A16:
	case AM_R_D16:
		sprintf(inst, "%-4s %s,$%04X",
			instName(_cur_inst->type),
			rtName(_cur_inst->reg1),
			_fetchData
		);
		break;
	case AM_R:
		sprintf(inst, "%-4s %s",
			instName(_cur_inst->type),
			rtName(_cur_inst->reg1)
		);
		break;
	case AM_R_R:
		sprintf(inst, "%-4s %s,%s",
			instName(_cur_inst->type),
			rtName(_cur_inst->reg1), rtName(_cur_inst->reg2)
		);
		break;
	case AM_MR_R:
		sprintf(inst, "%-4s (%s),%s",
			instName(_cur_inst->type),
			rtName(_cur_inst->reg1), rtName(_cur_inst->reg2)
		);
		break;
	case AM_MR:
		sprintf(inst, "%-4s (%s)",
			instName(_cur_inst->type),
			rtName(_cur_inst->reg1)
		);
		break;
	case AM_R_MR:
		sprintf(inst, "%-4s %s,(%s)",
			instName(_cur_inst->type),
			rtName(_cur_inst->reg1), rtName(_cur_inst->reg2)
		);
		break;
	case AM_R_D8:
	case AM_R_A8:
		sprintf(inst, "%-4s %s,$%02X",
			instName(_cur_inst->type),
			rtName(_cur_inst->reg1),
			_fetchData
		);
		break;
	case AM_R_HLI:
		sprintf(inst, "%-4s %s,(%s+)",
			instName(_cur_inst->type),
			rtName(_cur_inst->reg1), rtName(_cur_inst->reg2)
		);
		break;
	case AM_R_HLD:
		sprintf(inst, "%-4s %s,(%s-)",
			instName(_cur_inst->type),
			rtName(_cur_inst->reg1), rtName(_cur_inst->reg2)
		);
		break;
	case AM_HLI_R:
		sprintf(inst, "%-4s (%s+),%s",
			instName(_cur_inst->type),
			rtName(_cur_inst->reg1), rtName(_cur_inst->reg2)
		);
		break;
	case AM_HLD_R:
		sprintf(inst, "%-4s (%s-),%s",
			instName(_cur_inst->type),
			rtName(_cur_inst->reg1), rtName(_cur_inst->reg2)
		);
		break;
	case AM_A8_R:
		sprintf(inst, "%-4s $%02X,%s",
			instName(_cur_inst->type),
			read(PC - 1),
			rtName(_cur_inst->reg2)
		);
		break;
	case AM_HL_SPR:
		sprintf(inst, "%-4s (%s),SP+%d",
			instName(_cur_inst->type),
			rtName(_cur_inst->reg1),
			_fetchData & 0xFF
		);
		break;
	case AM_D8:
		sprintf(inst, "%-4s $%02X",
			instName(_cur_inst->type),
			_fetchData & 0xFF
		);
		break;
	case AM_D16:
		sprintf(inst, "%-4s $%04X",
			instName(_cur_inst->type),
			_fetchData
		);
		break;
	case AM_MR_D8:
		sprintf(inst, "%-4s (%s),$%02X",
			instName(_cur_inst->type),
			rtName(_cur_inst->reg1),
			_fetchData & 0xFF
		);
		break;
	case AM_D16_R:
	case AM_A16_R:
		sprintf(inst, "%-4s ($%04X),%s",
			instName(_cur_inst->type),
			_fetchData,
			rtName(_cur_inst->reg2)
		);
		break;
	}
	printf("%04X:%-16s (%02X %02X %02X) A:%02X F:%s BC:%02X%02X DE:%02X%02X HL:%02X%02X SP:%04X\n",
			PC,
			inst,
			this->_opcode,
			read(PC + 1),
			read(PC + 2),
			Cpu::regs.a,
			flags,
			Cpu::regs.b,
			Cpu::regs.c,
			Cpu::regs.d,
			Cpu::regs.e,
			Cpu::regs.h,
			Cpu::regs.l,
			Cpu::regs.sp
	);
}

void	Cpu::printDebug() {
	dprintf(STDOUT_FILENO,
		"A: %02X F: %02X B: %02X C: %02X "
		"D: %02X E: %02X H: %02X L: %02X "
		"SP: %04X PC: 00:%04X "
		"(%02X %02X %02X %02X)\n",
		regs.a,
		regs.f,
		regs.b,
		regs.c,
		regs.d,
		regs.e,
		regs.h,
		regs.l,
		regs.sp,
		regs.pc,
		read(regs.pc + 0),
		read(regs.pc + 1),
		read(regs.pc + 2),
		read(regs.pc + 3)
	);
}