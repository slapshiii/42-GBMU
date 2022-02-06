#include "Gbmu.hpp"

Gbmu::Gbmu(){
	_is_paused = false;
	_is_running = true;
}
Gbmu::~Gbmu(){}

void Gbmu::mem_write(uint16_t addr, uint8_t data) {
	_bus.write(addr, data);
}

uint8_t Gbmu::mem_read(uint16_t addr) {
	return _bus.read(addr);
}

void	Gbmu::mem_dump(std::ostream &o, uint16_t addr, size_t len) {
	size_t	i, j;
	if (len == 0)
		return;
	o << std::hex << std::uppercase << std::setfill('0');
	for (i = 0; i < len; i += 16) {
		o << std::setw(4) << addr + i << ": ";
		for (j = 0; (j < 16) && ((i + j) < len); ++j) {
			o << std::setw(2) << static_cast<unsigned int>(mem_read(addr + i + j)) << ' ';
		}
		for (;j < 16; ++j)
			o << " ";
		o << "- ";
		for (j = 0; (j < 16) && ((i + j) < len); ++j) {
			char c = static_cast<char>(mem_read(addr + i + j));
			if (c >= 0x20 && c < 0x7F)
				o << c;
			else
				o << '.';
		}
		o << std::endl;
	}
}

int	Gbmu::gbmu_runCL(int ac, char** av)
{
	if (ac != 2) {
		std::cerr << "Invalid argument: Usage: gbmu <rom file>" << std::endl;
		return (-1);
	}
	if (!this->_rom.romLoad(av[1])) {
		std::cerr << "Error: Failed to load ROM: " << av[1] << std::endl;
		return (-2);
	}
	std::cerr << "ROM loaded... : " << av[1] << std::endl;
	_cgb_mode = _rom.isCGB();
	while (this->_is_running)
	{
		if (this->_is_paused) {
			sleep(1);
			continue;
		}
		if (!this->_cpu.step()) {
			std::cerr << "Error: CPU stopped" << std::endl;
			return (-3);
		}
	}
	return (0);
}

void Gbmu::cycle(size_t n) {
	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			++_ticks;
			if (_timer.step())
				_cpu.requestInt(IT_TIMER);
			_ppu.step();
		}
		//DMAstep TODO
	}
}

bool	Gbmu::loadCartrige(const std::string &path) {
	_isRomLoaded = _rom.romLoad(path.c_str());
	return _isRomLoaded;
}
