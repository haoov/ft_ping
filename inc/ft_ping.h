/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 13:31:53 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/12 18:06:52 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING
#define FT_PING

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
#include <sys/time.h>

/* Types */
typedef unsigned char	byte_t;
typedef unsigned short	byte2_t;
typedef unsigned int	byte4_t;

/* Error */
#define ERR_NO_HOST		"missing host operand"
#define ERR_UNK_HOST	"unknown host"
#define ERR_MALLOC		"malloc error"

/* Sizes */
#define ICMP_HD_SIZE		0x8
#define ICMP_DEF_PKT_SIZE	0x40

struct ft_ping
{
	const char		*hostname;
	int				sockfd;
	struct sockaddr	dst;
	byte_t			*icmp_pkt;
	int				pkt_size;
	int				npkt_sent;
	int				npkt_rec;
	byte2_t			seq;
};

/* Functions declaration */
void	init(struct ft_ping *ft_ping, const char *hostname);
void	clean_all(int status, void *data);
void	print_ping(const char *hostname, struct sockaddr dst, int data_size);
void	stop_program(int sig);
void	build_icmp_packet(byte_t *icmp_pkt, byte2_t seq, int size);
void	print_stats(struct ft_ping *ft_ping);
void	send_icmp_echo(struct ft_ping *ft_ping);

#endif
