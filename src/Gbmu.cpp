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
	return;
}
void Gbmu::write16(uint16_t addr, uint8_t value) {
	this->write(addr + 1, (value >> 8) & 0xFF);
	this->write(addr, value & 0xFF);
}

uint8_t Gbmu::read(uint16_t addr) {
	if (addr < 0x8000)
		return (this->_rom.read(addr));
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
