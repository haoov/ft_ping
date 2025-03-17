#include "argparser.h"

int	main(int ac, const char **av)
{
	struct argparser	p;
	struct arg			*arg;

	exparg("host", 0, NULL, STR_T);
	exparg("verbose", 'v', "verbose", BOOL_T);
	exparg("help", '?', "help", BOOL_T);
	exparg("size", 's', "size", INT_T);


	p = parse_args(av);

	arg = get_arg(p.args, "host");
	if (arg) {printf("host: %s\n", arg->val.pval);}
	arg = get_arg(p.args, "verbose");
	if (arg) {printf("verbose: %d\n", arg->val.ival);}
	arg = get_arg(p.args, "help");
	if (arg) {printf("help: %d\n", arg->val.ival);}
	arg = get_arg(p.args, "size");
	if (arg) {printf("size: %d\n", arg->val.ival);}

	free_args(p.args);
	return 0;
}
