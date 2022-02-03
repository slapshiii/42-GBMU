#include "Gmbu.hpp"

Gbmu::Gbmu()
{
	ctx.paused = false;
	ctx.running = true;
	ctx.ticks = 0;
}

Gbmu::~Gbmu()
{
}

void Gbmu::write(uint16_t addr, uint8_t data) {
	if (addr < 0x8000)
		this->_rom.write(addr, data);
	else if (addr < 0xA000)	// Char/Map Data
		;
	else if (addr < 0xC000)	// ROM Ram
		this->_rom.write(addr, data);
	else if (addr < 0xE000)	// Working Ram (WRAM)
		this->_ram.wramWrite(addr, data);
	else if (addr < 0xFE00)	// Prohibited memory
		return;
	else if (addr < 0xFEA0)	// OAM
		;
	else if (addr < 0xFF00)	// Prohibited memory
		return;
	else if (addr < 0xFF80)	// IO Register
		;
	else if (addr < 0xFFFF)	// High Ram (HRAM)
		this->_ram.hramWrite(addr, data);
	else if (addr == 0xFFFF)// Interrupt Register
		this->_cpu.setIeReg(data);
	return;
}
void Gbmu::write16(uint16_t addr, uint8_t value) {
	this->write(addr + 1, (value >> 8) & 0xFF);
	this->write(addr, value & 0xFF);
}

uint8_t Gbmu::read(uint16_t addr) {
	if (addr < 0x8000)		// ROM Data
		return (this->_rom.read(addr));
	else if (addr < 0xA000)	// Char/Map Data
		;
	else if (addr < 0xC000)	// ROM Ram
		return (this->_rom.read(addr));
	else if (addr < 0xE000)	// Working Ram (WRAM)
		return (this->_ram.wramRead(addr));
	else if (addr < 0xFE00)	// Prohibited memory
		return 0;
	else if (addr < 0xFEA0)	// OAM
		;
	else if (addr < 0xFF00)	// Prohibited memory
		return 0;
	else if (addr < 0xFF80)	// IO Register
		;
	else if (addr < 0xFFFF)	// High Ram (HRAM)
		return (this->_ram.hramRead(addr));
	else if (addr == 0xFFFF)// Interrupt Register
		return (this->_cpu.getIeReg());
	dprintf(STDERR_FILENO, "Trying to read not implemented memory: %04X\nExiting...\n", addr);
	exit(1);
}
uint16_t Gbmu::read16(uint16_t addr) {
	uint16_t lo = this->read(addr);
	uint16_t hi = this->read(addr + 1);
	return (lo | (hi << 8));
}

void Gbmu::stackPush(uint8_t data) {
	--this->_cpu.regs.sp;
	this->write(this->_cpu.regs.sp, data);
}
void Gbmu::stackPush16(uint16_t data) {
	this->stackPush((data >> 8) & 0xFF);
	this->stackPush(data & 0xFF);
}
uint8_t		Gbmu::stackPop() {
	return (this->read(this->_cpu.regs.sp++));
}
uint16_t	Gbmu::stackPop16() {
	uint16_t lo = this->stackPop();
	uint16_t hi = this->stackPop();
	return ((hi << 8) | lo);
}

Cpu::cpuRegs Cpu::regs = (cpuRegs){0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int	Gbmu::gbmu_run(int ac, char** av)
{
	if (ac != 2) {
		std::cerr << "Invalid argument: Usage: gbmu <rom file>" << std::endl;
		return (-1);
	}
	if (!this->_rom.romLoad(av[1])) {
		std::cerr << "Error: Failed to load ROM: " << av[1] << std::endl;
		return (-2);
	}
	std::cout << "ROM loaded... : " << av[1] << std::endl;
	while (this->ctx.running)
	{
		if (this->ctx.paused) {
			sleep(1);
			continue;
		}
		if (!this->_cpu.step()) {
			std::cerr << "Error: CPU stopped" << std::endl;
			return (-3);
		}
		++this->ctx.ticks;
	}
	return (0);
}

Gbmu::gbmu_context* Gbmu::getContext() { return &ctx; };
