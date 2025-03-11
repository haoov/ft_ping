/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stats.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 09:24:01 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/11 09:59:10 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"

void	print_ping(const char *hostname, struct sockaddr dst)
{
	struct sockaddr_in	*addr_n;
	char				addr_p[16];

	addr_n = (struct sockaddr_in*)&dst;
	if (inet_ntop(
		AF_INET,
		&addr_n->sin_addr,
		addr_p,
		sizeof(struct sockaddr_in)
		) == NULL)
	{
		errx(EXIT_FAILURE, "%s", strerror(errno));
	}
	printf("PING %s (%s)\n", hostname, addr_p);
}
