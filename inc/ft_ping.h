/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 13:31:53 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/13 18:12:59 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
#define FT_PING_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <err.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <sys/time.h>
#include <stdint.h>
#include <math.h>

/* Errors */
#define ERR_NO_HOST		"missing host operand"
#define ERR_UNK_HOST	"unknown host"
#define ERR_MALLOC		"malloc error"

#define ERR_TIMEO		"timeout"
#define ERR_SMALL		"packet too small"

/* Sizes */
#define ICMP_HD_SIZE		0x8
#define ICMP_DEF_PKT_SIZE	0x40
#define IP_MAX_HD_SIZE		0x3C

#define MAXRTTVAL			0x64

/* Tiemout */
#define DEF_TIMEOUT	0x1

/* Types */

typedef struct sockaddr	ping_addr;

struct ping {
	const char	*hostname;
	int			sockfd;
	ping_addr	dst;
	uint8_t		*icmp_pkt;
	int			pkt_size;
	uint16_t	seq;
	useconds_t	stime;
	int		npkt_sent;
	int		npkt_recv;
	double	rttmin;
	double	rttmax;
	double	rtts[MAXRTTVAL];
};

/* Functions declaration */
void	init(struct ping *ping, const char *hostname);
void	clean_all(int status, void *data);
void	print_ping(const char *hostname, struct sockaddr dst, int data_size);
void	stop_program(int sig);
void	ping_stats(struct ping *ping);
void	ping_echo(struct ping *ping);
void	ping_reply(struct ping *ping);
void	reply_stats(struct ping *ping, ssize_t size);

#endif
