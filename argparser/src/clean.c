/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 14:10:32 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/17 18:21:35 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argparser.h"

void	free_parser(struct argparser *p)
{
	struct arg	*next;

	while (p->args)
	{
		next = p->args->next;
		free(p->args);
		p->args = next;
	}
	free(p);
}

