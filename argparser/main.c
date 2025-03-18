#include "argparser.h"
#include <ctype.h>
#include <stdbool.h>

int	main(int ac, const char **av)
{
	struct argparser	*p;
	const char			*host;
	int					help;
	int					verbose;
	int					size;
	double				interval;

	p = new_parser();
	if (!p)
	{
		exit(1);
	}
	add_argument(p, "host", 0, NULL, STR_T, (argval_t)"test_host");
	add_argument(p, "verbose", 'v', "verbose", BOOL_T, (argval_t)false);
	add_argument(p, "help", '?', "help", BOOL_T, (argval_t)false);
	add_argument(p, "size", 's', "size", INT_T, (argval_t)56);
	add_argument(p, "interval", 'i', "interval", FLOAT_T, (argval_t)1.0);
	parse_args(p, av);

	host = get_strarg(p, "host");
	help = get_intarg(p, "help");
	verbose = get_intarg(p, "verbose");
	size = get_intarg(p, "size");
	interval = get_darg(p, "interval");

	printf("host: %s\nhelp: %d\nverbose: %d\nsize: %d\ninterval: %f\n",
			host, help, verbose, size, interval);

	free_parser(p);
	return 0;
}
