/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 09:01:56 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/13 16:54:19 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"

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

void init(struct ping *ping, const char *hostname) {
	struct timeval	timeout;

	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	memset(ping, 0, sizeof(struct ping));
	signal(SIGINT, stop_program);
	on_exit(clean_all, ping);
	ping->pkt_size = ICMP_DEF_PKT_SIZE;
	ping->hostname = hostname;
	ping->sockfd = create_socket(timeout);
	ping->dst = resolve_hostname(ping->hostname);
	ping->icmp_pkt = malloc_pkt_buffer(ping->pkt_size);
}

