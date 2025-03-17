/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 09:01:56 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/17 11:38:40 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"

extern struct ping	ping;

/*
 * Create raw socket and set input timeout
 * */
int create_socket(struct timeval to) {
	int	sockfd;

	sockfd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd == -1) {
		errx(EXIT_FAILURE, "%s", strerror(errno));
	}
	(void)to;
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &to, sizeof(to)) == -1) {
		errx(EXIT_FAILURE, "%s", strerror(errno));
	}
	return sockfd;
}

/*
 * Resolve hostname to sockaddr struct
 * */
struct sockaddr resolve_hostname(const char *hostname) {
	struct addrinfo	*res, hint;
	struct sockaddr	dst;
	int				ret;

	res = NULL;
	memset(&hint, 0, sizeof(struct addrinfo));
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_RAW;
	hint.ai_protocol = IPPROTO_ICMP;
	ret = getaddrinfo(hostname, NULL, &hint, &res);
	if (ret == -1) {
		errx(EXIT_FAILURE, "%s", gai_strerror(errno));
	}
	if (!res) {
		errx(EXIT_FAILURE, "%s", ERR_UNK_HOST);
	}
	if (memcpy(&dst, res->ai_addr, sizeof(struct sockaddr)) == NULL) {
		errx(EXIT_FAILURE, "%s", strerror(errno));
	}
	freeaddrinfo(res);
	return dst;
}

/*
 * Malloc 'size' bytes for icmp packet buffer
 * */
uint8_t *malloc_pkt_buffer(int size) {
	uint8_t	*buffer;

	buffer = calloc(sizeof(uint8_t), size + IP_MAX_HD_SIZE);
	if (!buffer) {
		errx(EXIT_FAILURE, "%s", ERR_MALLOC);
	}
	return buffer;
}

void	init_args(const char **argv)
{
	struct argparser	parser;

	parser.args = NULL;
	exparg("host", 0, NULL, STR_T);
	exparg("help", '?', "help", BOOL_T);
	exparg("verbose", 'v', "verbose", BOOL_T);
	parser = parse_args(argv);
	if (parser.err)
	{
		exit(EXIT_FAILURE);
	}
	if (get_arg(parser.args, "help"))
	{
		print_help();
		exit(EXIT_SUCCESS);
	}
	ping.host = get_arg(parser.args, "host");
	if (!ping.host)
	{
		errx(EXIT_FAILURE, "%s", ERR_NO_HOST);
	}
}

void init(const char **argv) {
	struct timeval	timeout;

	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	signal(SIGINT, stop_program);
	init_args(argv);
	ping.pkt_size = ICMP_DEF_PKT_SIZE;
	ping.sockfd = create_socket(timeout);
	ping.dst = resolve_hostname(ping.p_host);
	ping.icmp_pkt = malloc_pkt_buffer(ping.pkt_size);
}

