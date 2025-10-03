#ifndef FT_PING_H
#define FT_PING_H

#include "unistd.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"
#include "errno.h"
#include "stdio.h"

struct ptrlist {
	void			*ptr;
	struct ptrlist	*next;
};

struct opt {
	const char	*lgopt;
	const char	shopt;
	enum {
		boolean,
		number,
		string
	} type;
	union {
		int		intgr;
		double	dbl;
		char	*ptr;
	} val;
};

struct strlist {
	const char		*str;
	struct strlist	*next;
};

struct ping {
	struct strlist	*hosts;
	struct opt		opts[26];
};

void		ping_error(const char *fmt, ...);
void		gc_add(void *ptr);
void		gc_free();
void		parse_args(const char **argv);

#endif
