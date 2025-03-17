/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 09:00:11 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/17 11:34:48 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"

struct ping	ping;

int main(const int argc, const char **argv)
{
	(void)argc;
	atexit(clean_all);
	init(argv);
	print_ping(ping.p_host, ping.dst, ping.pkt_size - ICMP_HD_SIZE);
	while (true)
	{
		ping_echo(&ping);
		ping_reply(&ping);
		usleep(ping.stime);
	}
	return EXIT_SUCCESS;
}

