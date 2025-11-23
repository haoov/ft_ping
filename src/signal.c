#include "ft_ping.h"

extern struct ping p;
volatile sig_atomic_t interrupt = false;

void handle_sigint(int signum) {
	(void)signum;
	
	interrupt = true;
}
