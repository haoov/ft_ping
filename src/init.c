/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 09:01:56 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/17 22:26:44 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"

extern struct ping	ping;

/*
 * Create raw socket and set input timeout
 * */
int create_socket(struct timeval to)
{
	int	sockfd;

	sockfd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd == -1)
	{
		errx(EXIT_FAILURE, "%s", strerror(errno));
	}
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &to, sizeof(to)) == -1)
	{
		errx(EXIT_FAILURE, "%s", strerror(errno));
	}
	return sockfd;
}

/*
 * Resolve hostname to sockaddr struct
 * */
struct sockaddr resolve_hostname(const char *hostname)
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

/*
 * Malloc 'size' bytes for icmp packet buffer
 * */
uint8_t *malloc_pkt_buffer(int size)
{
	uint8_t	*buffer;

	buffer = calloc(size + IP_MAX_HD_SIZE, sizeof(uint8_t));
	if (!buffer)
	{
		errx(EXIT_FAILURE, "%s", ERR_MALLOC);
	}
	return buffer;
}

void	init_args(const char **argv)
{
	ping.parser = new_parser();
	add_argument(ping.parser, "host", 0, NULL, STR_T, (argval_t)0);
	add_argument(ping.parser, "help", '?', "help", BOOL_T, (argval_t)false);
	add_argument(ping.parser, "verbose", 'v',
					"verbose", BOOL_T, (argval_t)false);
	add_argument(ping.parser, "size", 's',
					"size", INT_T, (argval_t)ICMP_DEF_DATA_SIZE);
	add_argument(ping.parser, "count", 'c', "count", INT_T, (argval_t)0);
	add_argument(ping.parser, "linger", 'W', "linger", INT_T, (argval_t)1);
	parse_args(ping.parser, argv);
	if (ping.parser->err)
	{
		exit(EXIT_FAILURE);
	}
}

void	check_args(struct ping ping)
{
	if (get_intarg(ping.parser->args, "help"))
	{
		print_help();
		exit(EXIT_SUCCESS);
	}
	if (!ping.host)
	{
		errx(EXIT_FAILURE, "%s", ERR_NO_HOST);
	}
	if (ping.data_size < 0)
	{
		errx(EXIT_FAILURE, "%s `%d`", ERR_INPKTSIZE, ping.data_size);
	}
	if (ping.linger <= 0)
	{
		errx(EXIT_FAILURE, "%s `%d`", ERR_TOSMALL, ping.linger);
	}
}

void init(const char **argv)
{
	struct timeval	timeout;

	timeout.tv_usec = 0;
	signal(SIGINT, stop_program);
	init_args(argv);
	ping.host = get_strarg(ping.parser->args, "host");
	ping.data_size = get_intarg(ping.parser->args, "size");
	ping.count = get_intarg(ping.parser->args, "count");
	ping.linger = get_intarg(ping.parser->args, "linger");
	check_args(ping);
	timeout.tv_sec = ping.linger;
	ping.sockfd = create_socket(timeout);
	ping.dst = resolve_hostname(ping.host);
	ping.icmp_pkt = malloc_pkt_buffer(ping.data_size + ICMP_HD_SIZE);
}

