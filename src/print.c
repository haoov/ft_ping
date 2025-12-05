#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void ping_error(const char *fmt, ...) {
	if (!fmt) {
		return;
	}
	va_list ap;
	va_start(ap, fmt);
	fprintf(stderr, "ft_ping: ");
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fprintf(stderr, "Try `ping --help` for more information.\n");
	exit(1);
}
