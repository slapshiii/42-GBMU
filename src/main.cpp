#include "Gbmu.hpp"

#include <signal.h>

Gbmu gbmu;

void	siginthanlder(int sig){
	(void)sig;
	gbmu.stop();
}

int main(int ac, char **av)
{
	sighandler_t prevsig;
	prevsig = signal(SIGINT, &siginthanlder);
	gbmu.gbmu_runCL(ac, av);
	signal(SIGINT, prevsig);
	return (0);
}