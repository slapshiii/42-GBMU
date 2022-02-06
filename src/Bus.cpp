#include "Bus.hpp"

Bus::Bus(Rom &r, Cpu &c, Ppu &p, Timer &t) : _rom(r), _cpu(c), _ppu(p), _timer(t)
{}

Bus::~Bus() {}

void Bus::write(uint16_t addr, uint8_t data) {
	//if (addr == 0xDD03)
	///	dprintf(STDERR_FILENO, "cycle: %d[W] -- op:%02X\n", _cpu.cycles, _cpu._opcode);
	if (addr < 0x8000)
		_rom.write(addr, data);
	else if (addr < 0xA000)	// Char/Map Data
		this->_ppu.writeVram(addr, data);
	else if (addr < 0xC000)	// ROM Ram
		_rom.write(addr, data);
	else if (addr < 0xE000)	// Working Ram (WRAM)
		_wram[addr - 0xC000] = data;
	else if (addr < 0xFE00)	// Prohibited memory
		return;
	else if (addr < 0xFEA0)	// OAM
		this->_ppu.writeOam(addr, data);
	else if (addr < 0xFF00)	// Prohibited memory
		return;
	else if (addr < 0xFF80)	// IO Register
	{
		if (addr == 0xFF00) //Gamepad TODO
			return ;
		else if (addr == 0xFF01)
			_io[0] = data;
		else if (addr == 0xFF02)
			_io[1] = data;
		else if (BETWEEN(addr, 0xFF04, 0xFF07))
			this->_timer.write(addr, data);
		else if (addr == 0xFF0F)
			this->_cpu.setIntFlags(data);
		else if (BETWEEN(addr, 0xFF10, 0xFF3F)) //Sound TODO
			return ;
		else if (BETWEEN(addr, 0xFF40, 0xFF4B))
			this->_ppu.write(addr, data);
	}
	else if (addr < 0xFFFF)	// High Ram (HRAM)
		_hram[addr - 0xFF80] = data;
	else if (addr == 0xFFFF)// Interrupt Register
		this->_cpu.setIeReg(data);
	return;
}

uint8_t Bus::read(uint16_t addr) {
	//if (addr == 0xDD03)
	//	dprintf(STDERR_FILENO, "cycle: %d[R] -- op:%02X\n", _cpu.cycles, _cpu._opcode);
	if (addr < 0x8000)		// ROM Data
		return (this->_rom.read(addr));
	else if (addr < 0xA000)	// Char/Map Data
		return (this->_ppu.readVram(addr));
	else if (addr < 0xC000)	// ROM Ram
		return (this->_rom.read(addr));
	else if (addr < 0xE000)	// Working Ram (WRAM)
		return (_wram[addr - 0xC000]);
	else if (addr < 0xFE00)	// Prohibited memory
		return 0;
	else if (addr < 0xFEA0)	// OAM
		return (this->_ppu.readOam(addr));
	else if (addr < 0xFF00)	// Prohibited memory
		return 0;
	else if (addr < 0xFF80)	// IO Register
	{
		if (addr == 0xFF00) //Gamepad TODO
			return 0;
		if (addr == 0xFF01)
			return _io[0];
		if (addr == 0xFF02)
			return _io[1];
		if (BETWEEN(addr, 0xFF04, 0xFF07))
			return this->_timer.read(addr);
		if (addr == 0xFF0F)
			return this->_cpu.getIntFlags();
		if (BETWEEN(addr, 0xFF10, 0xFF3F)) //Sound TODO
			return 0;
		if (BETWEEN(addr, 0xFF40, 0xFF4B)) //LCD TODO
			return _ppu.read(addr);
	}
	else if (addr < 0xFFFF)	// High Ram (HRAM)
		return (_hram[addr - 0xFF80]);
	else if (addr == 0xFFFF)// Interrupt Register
		return (_cpu.getIeReg());
	return 0;
}