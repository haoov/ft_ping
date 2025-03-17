/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_err.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:00:18 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/16 14:26:46 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argparser.h"

extern char	*__progname;

void	arg_err(const char *fmt, ...)
{
	va_list	ap;

	va_start(ap, fmt);
	fprintf(stderr, "%s: ", __progname);
	if (fmt)
	{
		vfprintf(stderr, fmt, ap);
	}
	fprintf(stderr, "\n");
	va_end(ap);
}

