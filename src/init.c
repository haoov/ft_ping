/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 09:01:56 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/11 09:22:33 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"

int	create_socket()
{
	int	sockfd;

	sockfd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd == -1)
	{
		errx(EXIT_FAILURE, "%s", strerror(errno));
	}
	return sockfd;
}

struct sockaddr	resolve_hostanme(const char *hostname)
{
	struct addrinfo	*res, hint;
	struct sockaddr	dst;
	int				ret;

	res = NULL;
	memset(&hint, 0, sizeof(struct addrinfo));
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_RAW;
	hint.ai_protocol = IPPROTO_ICMP;
	ret = getaddrinfo(hostname, NULL, &hint, &res);
	if (ret == -1)
	{
		errx(EXIT_FAILURE, "%s", gai_strerror(errno));
	}
	if (!res)
	{
		errx(EXIT_FAILURE, "%s", ERR_UNK_HOST);
	}
	if (memcpy(&dst, res->ai_addr, sizeof(struct sockaddr)) == NULL)
	{
		errx(EXIT_FAILURE, "%s", strerror(errno));
	}
	freeaddrinfo(res);
	return dst;
}

