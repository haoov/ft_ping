/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 14:10:32 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/17 10:07:55 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argparser.h"

extern struct exparg	*explist;

void	free_expargs()
{
	struct exparg	*args, *next;

	args = explist;
	while (args)
	{
		next = args->next;
		free(args);
		args = next;
	}
}

void	free_args(struct arg *args)
{
	struct arg	*next;

	free_expargs();
	while (args)
	{
		next = args->next;
		free(args);
		args = next;
	}
}

