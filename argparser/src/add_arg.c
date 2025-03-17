/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_arg.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:10:18 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/14 15:47:59 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argparser.h"

/* Add argument 'arg' at the end of list 'list'
 * Return pointer to head of list
 * */
struct arg	*add_arg(struct arg **list, struct arg *arg)
{
	struct arg	*cur;

	if (!list)
	{
		return arg;
	}
	cur = *list;
	while (cur->next)
	{
		cur = cur->next;
	}
	cur->next = arg;
	return *list;
}
