/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 09:00:11 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/17 20:52:10 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"

struct ping	ping;

int main(const int argc, const char **argv)
{
	(void)argc;
	atexit(clean_all);
	init(argv);
	print_ping(ping);
	while (true)
	{
		icmp_request(&ping);
		icmp_reply(&ping);
		usleep(ping.stime);
	}
	return EXIT_SUCCESS;
}

