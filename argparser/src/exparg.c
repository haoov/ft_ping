/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exparg.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:23:37 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/17 10:50:01 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argparser.h"

struct exparg	*explist = NULL;

struct exparg	*new_exparg(char *name, char shval, char *lgval, int type)
{
	struct exparg	*new;

	new = calloc(1, sizeof(struct exparg));
	if (!new)
	{
		return NULL;
	}
	new->name = name;
	new->shval = shval;
	new->lgval = lgval;
	new->type = type;
	new->next = NULL;
	return new;
}

void	add_exparg(struct exparg *arg)
{
	struct exparg	*cur;

	if (!explist)
	{
		explist = arg;
	}
	else
	{
		cur = explist;
		while (cur->next)
		{
			cur = cur->next;
		}
		cur->next = arg;
	}
}

int	exparg(char *name, char shval, char *lgval, int type)
{
	struct exparg	*arg;

	arg = new_exparg(name, shval, lgval, type);
	if (!arg)
	{
		return -1;
	}
	add_exparg(arg);
	return 0;
}
