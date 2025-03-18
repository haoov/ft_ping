/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 20:14:21 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/18 11:49:22 by rsabbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argparser.h"

double	get_darg(struct argparser *p, const char *name)
{
	struct arg *args;

	args = p->args;
	while (args)
	{
		if (args->type == FLOAT_T && !strcmp(args->name, name))
		{
			return args->val.dval;
		}
		args = args->next;
	}
	return 0.0;
}

const char	*get_strarg(struct argparser *p, const char *name)
{
	struct arg	*args;

	args = p->args;
	while (args)
	{
		if (args->type == STR_T && !strcmp(args->name, name))
		{
			return args->val.pval;
		}
		args = args->next;
	}
	return NULL;
}

int	get_intarg(struct argparser *p, const char *name)
{
	struct arg	*args;

	args = p->args;
	while (args)
	{
		if ((args->type == INT_T || args->type == BOOL_T) &&
				!strcmp(args->name, name))
		{
			return args->val.ival;
		}
		args = args->next;
	}
	return 0;
}

void	arglist_add(struct arg **list, struct arg *arg)
{
	struct arg	*cur;

	if (!(*list))
	{
		*list = arg;
	}
	else
	{
		cur = *list;
		while (cur->next)
		{
			cur = cur->next;
		}
		cur->next = arg;
	}
}

struct arg	*new_arg(char *name, char sh, char *lg, int type, argval_t def)
{
	struct arg	*new;

	new = calloc(1, sizeof(struct arg));
	if (!new)
	{
		return NULL;
	}
	new->name = name;
	new->shval = sh;
	new->lgval = lg;
	new->type = type;
	new->val = def;
	new->next = NULL;
	return new;
}

void	add_argument(struct argparser *p, char *name,
						char sh, char *lg, int type, argval_t def)
{
	struct arg	*arg;

	arg = new_arg(name, sh, lg, type, def);
	if (!arg)
	{
		p->err = true;
	}
	else
	{
		arglist_add(&p->args, arg);
	}
}
