#include "../inc/ft_ping.h"
#include <inttypes.h>

extern struct ping ping;

bool is_float(const char *val) {
	if (strchr(val, '.')) {
		return true;
	}
	return false;
}

int set_opt_val(struct opt *opt, const char *val) {
	if (opt->type == boolean) {
		opt->val.intgr = true;
	}
	else if (opt->type == number) {
		char *endptr;
		if (is_float(val)) {
			double dval = strtod(val, &endptr);
			opt->val.dbl = dval;
		}
		else {
			intmax_t ival = strtoimax(val, &endptr, 10);
			opt->val.intgr = (int)ival;
		}
		if (errno) {
			ping_error("%s: %s\n", val, strerror(errno));
		}
		if (*endptr && endptr != val) {
			return 1;
		}
	}
	else {
		opt->val.ptr = (char*)val;
	}
	return 0;
}

struct opt *get_opt(const char *lg, const char sh) {
	if (!lg && !sh) {
		return NULL;
	}
	for (int i = 0; ping.opts[i].lgopt; ++i) {
		if ((sh && sh == ping.opts[i].shopt)
			|| (lg && !strcmp(lg, ping.opts[i].lgopt))) {
			return &ping.opts[i];
		}
	}
	return NULL;
}

void add_host(const char *host) {
	if (!host) {
		return;
	}

	struct strlist *new = malloc(sizeof(struct strlist));
	if (!new) {
		ping_error("malloc error\n");
	}
	new->str = host;
	new->next = NULL;

	if (!ping.hosts) {
		ping.hosts = new;
	}
	else {
		struct strlist *elem = ping.hosts;
		while (elem->next) {
			elem = elem->next;
		}
		elem->next = new;
	}
}

void parse_args(int argc, const char **argv) {
	for (int i = 1; i < argc; ++i) {
		const char	*arg = argv[i];
		size_t		len = strlen(arg);

		if (arg[0] == '-' && len >= 2) {
			// arg is an option
			struct opt	*opt;
			const char	*val = argv[i + 1];

			if (arg[1] != '-') {
				// short format '-...'
				for (int j = 1; arg[j]; ++j) {
					opt = get_opt(NULL, arg[j]);
					if (!opt) {
						ping_error("unrecognized option: `-%c`\n", arg[j]);
					}
					if (opt->type != boolean) {
						if (arg[j + 1]) {
							ping_error("-%c invalid value: %c\n", arg[j], arg[j + 1]);
						}
						if (!val) {
							ping_error("option requires an argument `-%c`\n", arg[j]);
						}
						++i;
					}
					if (set_opt_val(opt, val) != 0) {
						ping_error("-%c: invalid value: %s\n", arg[j], val);
					}
				}
			}
			else if (len > 2) {
				// long format '--...'
				opt = get_opt(&arg[2], 0);

				if (!opt) {
					ping_error("unrecognized option: `%s`\n", arg);
				}
				if (!val && opt->type != boolean) {
					ping_error("option requires an argument `%s`\n", arg);
				}
				if (opt->type != boolean) {
					++i;
				}
				if (set_opt_val(opt, val) != 0) {
					ping_error("%s: invalid value: %s\n", arg, val);
				}
			}
			else {
				// arg is '--'
				continue;
			}
		}
		else {
			add_host(arg);
		}
	}
}
