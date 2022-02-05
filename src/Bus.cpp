#include "Bus.hpp"

Bus::Bus(Cpu &c, Ppu &p, Timer &t) : _cpu(c), _ppu(p), _timer(t)
{}

Bus::~Bus() {}

void Bus::write(uint16_t addr, uint8_t data) {
	if (addr < 0x8000)
		this->_rom.write(addr, data);
	else if (addr < 0xA000)	// Char/Map Data
		this->_ppu.writeVram(addr, data);
	else if (addr < 0xC000)	// ROM Ram
		this->_rom.write(addr, data);
	else if (addr < 0xE000)	// Working Ram (WRAM)
		this->_ram.wramWrite(addr, data);
	else if (addr < 0xFE00)	// Prohibited memory
		return;
	else if (addr < 0xFEA0)	// OAM
		this->_ppu.writeOam(addr, data);
	else if (addr < 0xFF00)	// Prohibited memory
		return;
	else if (addr < 0xFF80)	// IO Register
		this->writeIO(addr, data);
	else if (addr < 0xFFFF)	// High Ram (HRAM)
		this->_ram.hramWrite(addr, data);
	else if (addr == 0xFFFF)// Interrupt Register
		this->_cpu.setIeReg(data);
	return;
}

uint8_t Bus::read(uint16_t addr) {
	if (addr < 0x8000)		// ROM Data
		return (this->_rom.read(addr));
	else if (addr < 0xA000)	// Char/Map Data
		return (this->_ppu.readVram(addr));
	else if (addr < 0xC000)	// ROM Ram
		return (this->_rom.read(addr));
	else if (addr < 0xE000)	// Working Ram (WRAM)
		return (this->_ram.wramRead(addr));
	else if (addr < 0xFE00)	// Prohibited memory
		return 0;
	else if (addr < 0xFEA0)	// OAM
		return (this->_ppu.readOam(addr));
	else if (addr < 0xFF00)	// Prohibited memory
		return 0;
	else if (addr < 0xFF80)	// IO Register
		return (this->readIO(addr));
	else if (addr < 0xFFFF)	// High Ram (HRAM)
		return (this->_ram.hramRead(addr));
	else if (addr == 0xFFFF)// Interrupt Register
		return (this->_cpu.getIeReg());
	dprintf(STDERR_FILENO, "Trying to read not implemented memory: %04X\nExiting...\n", addr);
	exit(1);
}