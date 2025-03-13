/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 09:00:11 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/13 12:10:44 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"

int main(const int argc, const char **argv) {
	struct ping	ping;

	if (argc != 2) {
		errx(EXIT_FAILURE, "%s", ERR_NO_HOST);
	}
	init(&ping, argv[1]);
	print_ping(ping.hostname, ping.dst, ping.pkt_size - ICMP_HD_SIZE);
	while (true) {
		ping_echo(&ping);
		ping_reply(&ping);
		usleep(ping.stime);
	}
	return EXIT_SUCCESS;
}

