#ifndef _GBMU_HPP
#define _GBMU_HPP

#include <array>

#include "Ppu.hpp"
#include "Cpu.hpp"
#include "Rom.hpp"
#include "Timer.hpp"
#include "Lcd.hpp"
#include "Bus.hpp"
#include "utils.hpp"

class Ppu;
class Cpu;
class Timer;
class Bus;

class Gbmu
{
public:
	Gbmu();
	~Gbmu();

	//GBMU interface
	void		run_thread();
	void		stop();
	void		pause();
	void		resume();
	void		reset();

	void		step();
	void		exec();
	void		cycle(size_t n);

	bool		is_cgb();
	uint32_t	getTicks();

	uint8_t	mem_read(uint16_t addr);
	void	mem_write(uint16_t addr, uint8_t data);

	//ROM interface
	bool	loadCartrige(const std::string &path);
	bool	set_dmgForce(bool);
	bool	set_cgbMode(bool);

	int	gbmu_runCL(int ac, char** av);

private:

	//save the game (progress in pokemon)
	void 	saveGame(const std::vector<uint8_t> &sram);
	void	run();

private:

	bool		_is_paused;
	bool		_is_running;
	bool		_cgb_mode;
	bool		_dmg_force;
	uint64_t	_ticks;

	std::thread 			_emu_thread;
	std::condition_variable	_pause_cv;
	mutable	std::mutex		_mutex;

	std::string _saveName;

	bool	_isRomLoaded;
	Rom		_rom;
	Cpu		_cpu {
		[this](uint16_t addr){ return this->mem_read(addr); },
		[this](uint16_t addr, uint8_t data){ return this->mem_write(addr, data); },
		[this](size_t n){ return this->cycle(n); },
	};
	Ppu		_ppu { _bus, _cpu };
	Timer	_timer;
	Bus		_bus { _rom, _cpu, _ppu, _timer };

};

#endif