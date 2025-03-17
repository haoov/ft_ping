/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 20:14:21 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/17 19:55:10 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argparser.h"

const char	*get_strarg(struct arg *list, const char *name)
{
	while (list)
	{
		if (list->type == STR_T && !strcmp(list->name, name))
		{
			return list->val.pval;
		}
		list = list->next;
	}
	return NULL;
}

int	get_intarg(struct arg *list, const char *name)
{
	while (list)
	{
		if ((list->type == INT_T || list->type == BOOL_T) &&
				!strcmp(list->name, name))
		{
			return list->val.ival;
		}
		list = list->next;
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
