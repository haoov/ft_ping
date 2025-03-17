/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 20:14:21 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/16 14:39:45 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argparser.h"

struct arg	*get_arg(struct arg *list, const char *name)
{
	while (list)
	{
		if (!strcmp(list->name, name))
		{
			return list;
		}
		list = list->next;
	}
	return NULL;
}

void	add_arg(struct arg **list, struct arg *arg)
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

struct arg	*new_arg(const char *name, int type, union argval val)
{
	struct arg	*new;

	new = calloc(1, sizeof(struct arg));
	if (!new)
	{
		return NULL;
	}
	new->name = name;
	new->type = type;
	new->val = val;
	new->next = NULL;
	return new;
}

int	arg(struct arg **args, const char *name, int type, union argval val)
{
	struct arg	*arg;

	arg = new_arg(name, type, val);
	if (!arg)
	{
		return -1;
	}
	add_arg(args, arg);
	return 0;
}
