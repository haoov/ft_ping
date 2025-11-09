#include "ft_ping.h"

extern struct ping p;

void handle_sigint(int signum) {
	(void)signum;
	struct opt *count = get_opt(p.opts, "count", 0);
	
	count->val.intgr = 1;
}
