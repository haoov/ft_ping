#include "../inc/ft_ping.h"
#include <inttypes.h>

bool is_float(const char *val) {
	if (strchr(val, '.')) {
		return true;
	}
	return false;
}

// !: Not tested
// TODO: Test value checking
int check_value(struct opt *opt) {
	if (opt->type == floating) {
		if (opt->val.dbl <= 0) {
			return 1;
		}
	}
	else if (opt->type == number) {
		switch (opt->shopt) {
			case 'c':
			case 's':
				if (opt->val.intgr <= 0) {
					return 1;
				}
				break;
		}
	}
	return 0;
}

int set_opt_val(struct opt *opt, const char *val) {
	if (opt->type == boolean) {
		opt->val.intgr = true;
	}
	else if (opt->type == floating) {
		char *endptr;
		opt->val.dbl = strtod(val, &endptr);
		if (errno) {
			ping_error("%s: %s\n", val, strerror(errno));
		}
		if (*endptr && endptr != val) {
			return 1;
		}
	}
	else if (opt->type == number) {
		char *endptr;
		intmax_t ival = strtoimax(val, &endptr, 10);
		opt->val.intgr = (int)ival;
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
	return check_value(opt);
}

struct opt *get_opt(struct opt *opts, const char *lg, const char sh) {
	if (!lg && !sh) {
		return NULL;
	}
	for (int i = 0; opts[i].lgopt; ++i) {
		if ((sh && sh == opts[i].shopt)
			|| (lg && !strcmp(lg, opts[i].lgopt))) {
			return &opts[i];
		}
	}
	return NULL;
}

void add_host(struct ping *p, const char *host) {
	if (!host) {
		return;
	}
	if (p->host_count == MAX_HOST) {
		ping_error("too many hosts\n");
	}

	p->hosts[p->host_count] = (char*)host;
	++p->host_count;
}

void parse_args(struct ping *p, int argc, const char **argv) {
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
					opt = get_opt(p->opts, NULL, arg[j]);
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
				opt = get_opt(p->opts, &arg[2], 0);

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
			add_host(p, arg);
		}
	}
}
