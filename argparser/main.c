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

	p = new_parser();
	if (!p)
	{
		exit(1);
	}
	add_argument(p, "host", 0, NULL, STR_T, (argval_t)"test_host"),
	add_argument(p, "verbose", 'v', "verbose", BOOL_T, (argval_t)false),
	add_argument(p, "help", '?', "help", BOOL_T, (argval_t)false);
	add_argument(p, "size", 's', "size", INT_T, (argval_t)56),
	parse_args(p, av);

	host = get_strarg(p->args, "host");
	help = get_intarg(p->args, "help");
	verbose = get_intarg(p->args, "verbose");
	size = get_intarg(p->args, "size");

	printf("host: %s\nhelp: %d\nverbose: %d\nsize: %d\n",
			host, help, verbose, size);

	free_parser(p);
	return 0;
}
