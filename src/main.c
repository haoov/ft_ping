#include "../inc/ft_ping.h"

struct ping p = {
	.host_count = 0,
	.sendbuf = NULL,
	.recvbuf = NULL,
	.opts = {
		{"count", 'c', number, .val.intgr = 0},
		{"debug", 'd', boolean, .val.intgr = false},
		{"interval", 'i', floating, .val.dbl = 1.0},
		{"numeric", 'n', boolean, .val.intgr = false},
		{"ignore-routing", 'r', boolean, .val.intgr = false},
		{"ttl", 0, number, .val.intgr = 64},
		{"tos", 'T', number, .val.intgr = 0},
		{"verbose", 'v', boolean, .val.intgr = false},
		{"timeout", 'w', floating, .val.dbl = 0.0},
		{"linger", 'W', floating, .val.dbl = 10.0},
		{"help", '?', boolean, .val.intgr = false},
		{"usage", 0, boolean, .val.intgr = false},
		{"version", 'V', boolean, .val.intgr = false},
		{"flood", 'f', boolean, .val.intgr = false},
		{"ip-timestamp", 0, string, .val.ptr = NULL},
		{"preload", 'l', number, .val.intgr = 0},
		{"pattern", 'p', string, .val.ptr = NULL},
		{"quiet", 'q', boolean, .val.intgr = false},
		{"route", 'R', boolean, .val.intgr = false},
		{"size", 's', number, .val.intgr = 56},
		{NULL, 0, 0, .val.intgr = 0}
	},
};

int main(int argc, const char **argv) {
	signal(SIGINT, handle_sigint);
	atexit(ping_exit);
	parse_args(&p, argc, argv);
	if (!p.host_count) {
		ping_no_args(&p);
	}
	ping_init(&p);
	ft_ping(&p);
}
