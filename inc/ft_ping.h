/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 13:31:53 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/18 12:20:16 by rsabbah          ###   ########.fr       */
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
#include "argparser.h"

/* Errors */
#define ERR_NO_HOST		"missing host operand"
#define ERR_UNK_HOST	"unknown host"
#define ERR_TOSMALL		"value to small"
#define ERR_TOBIG		"value to big"
#define ERR_MALLOC		"malloc error"
#define ERR_INPKTSIZE	"invalid packet size"
#define ERR_NOFLOOD		"cannot flood; minimal interval allowed for user is 2ms"
#define ERR_BADINT		"bad interval value"

#define ERR_TIMEO		"timeout"
#define ERR_SMALL		"packet too small"

/* Sizes */
#define ICMP_HD_SIZE		0x8
#define ICMP_DEF_DATA_SIZE	0x38
#define IP_MAX_HD_SIZE		0x3C

#define	DEF_INTERVAL		0x1
#define DEF_TTL				0x3C

#define MAXRTTVAL			0x64
#define ADDRLEN				0xF

/* Tiemout */
#define DEF_TIMEOUT	0x1

/* Types */

struct ping
{
	struct argparser	*parser;
	const char			*host;
	int					sockfd;
	struct sockaddr		dst;
	char				addrstr[ADDRLEN];
	uint8_t				*icmp_pkt;
	int					data_size;
	uint16_t			seq;
	useconds_t			stime;
	int					npkt_sent;
	int					npkt_recv;
	int					count;
	int					linger;
	double				interval;
	double				rttmin;
	double				rttmax;
	double				rtts[MAXRTTVAL];
};

/* Functions declaration */
void	init(const char **argv);
void	print_ping(struct ping *ping);
void	stop_program(int sig);
void	ping_stats();
void	icmp_request(struct ping *ping);
void	icmp_reply(struct ping *ping);
void	reply_stats(uint8_t *buf, ssize_t size);
void	print_help();

void	clean_all();

double	reducef(double *arr, size_t size);

#endif
