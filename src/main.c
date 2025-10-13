#include "../inc/ft_ping.h"

struct ping ping = {
	.hosts = NULL,
	.opts = {
		{"address", 0, boolean, .val.intgr = false},
		{"echo", 0, boolean, .val.intgr = true},
		{"mask", 0, boolean, .val.intgr = false},
		{"timestamp", 0, boolean, .val.intgr = false},
		{"type", 't', string, .val.ptr = NULL},
		{"count", 'c', number, .val.intgr = 3},
		{"debug", 'd', boolean, .val.intgr = false},
		{"interval", 'i', number, .val.dbl = 1.0},
		{"numeric", 'n', boolean, .val.intgr = false},
		{"ignore-routing", 'r', boolean, .val.intgr = false},
		{"ttl", 0, number, .val.intgr = 64},
		{"tos", 'T', number, .val.intgr = 0},
		{"verbose", 'v', boolean, .val.intgr = false},
		{"timeout", 'w', number, .val.dbl = 0.0},
		{"linger", 'W', number, .val.dbl = 10.0},
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
	}
};

int main(int argc, const char **argv) {
	atexit(ping_exit);
	parse_args(argc, argv);
	if (!ping.hosts) {
		ping_error("missing host operand\n");
	}
	init_socket();
	init_buf();
	ft_ping();
}
