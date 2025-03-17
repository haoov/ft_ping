/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:48:48 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/17 10:48:45 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argparser.h"

extern struct exparg	*explist;

struct exparg	*get_exparg()
{
	struct exparg	*cur;

	cur = explist;
	while (cur)
	{
		if (!cur->shval && !cur->lgval &&!cur->found)
		{
			cur->found = 1;
			return cur;
		}
		cur = cur->next;
	}
	return NULL;
}

struct exparg	*get_expopt(const char *str)
{
	struct exparg	*cur;

	while (*str == '-')
	{
		++str;
	}
	cur = explist;
	while (cur)
	{
		if ((cur->lgval || cur->shval) &&
			(!strncmp(cur->lgval, str, strlen(cur->lgval)) ||
			*str == cur->shval))
		{
			return cur;
		}
		cur = cur->next;
	}
	return NULL;
}

union argval	get_optval(int type, const char **av, int *i, size_t pos)
{
	union argval	val;
	const char		*sval;

	if (type == BOOL_T)
	{
		val.ival = 1;
		return val;
	}
	sval = av[*i][pos] ? av[*i] + pos : av[++(*i)];
	if (type == INT_T)
	{
		val.ival = atoi(sval);
	}
	else
	{
		val.pval = sval;
	}
	return val;
}

int	parse_shopt(struct arg **args, const char **av, int *i)
{
	struct exparg	*exp;
	union argval	val;

	for (int j = 1; av[*i][j]; ++j)
	{
		exp = get_expopt(&av[*i][j]);
		if (!exp)
		{
			arg_err("%s '-%c'", PERR_INOPT, av[*i][j]);
			return -1;
		}
		val = get_optval(exp->type, av, i, j + 1);
		arg(args, exp->name, exp->type, val);
		if (exp->type != BOOL_T)
		{
			return 0;
		}
	}
	return 0;
}

int	parse_arg(struct arg **args, const char **av, int *i)
{
	struct exparg	*exp;
	union argval	val;

	if (IS_OPT(av[*i]))
	{
		if (av[*i][1] != '-')
		{
			return parse_shopt(args, av, i);
		}
		else
		{
			exp = get_expopt(av[*i]);
			if (!exp)
			{
				arg_err("%s '--%s'", PERR_INOPT, av[*i] + 2);
				return -1;
			}
		}
	}
	else
	{
		exp = get_exparg();
	}
	if (exp)
	{
		if (exp->lgval)
		{
			val = get_optval(exp->type, av, i, strlen(exp->lgval) + 2);
		}
		else
		{
			val.pval = av[*i];
		}
		return arg(args, exp->name, exp->type, val);
	}
	return 0;
}

struct argparser	parse_args(const char **av)
{
	struct argparser	p;

	p.args = NULL;
	p.err = 0;
	for (int i = 1; av[i]; ++i)
	{
		p.err = parse_arg(&p.args, av, &i);
		if (p.err)
		{
			break;
		}
	}
	return p;
}
