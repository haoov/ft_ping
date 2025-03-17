/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:48:26 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/17 10:01:12 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	print_help()
{
	printf("Usage: ft_ping [OPTIONS...] HOST ...\n");
	printf("Send ICMP ECHO_REQUEST packets to network hosts.\n\n");
	printf(" Options:\n");
	printf("  -c, --count=NUMBER\t\tstop after sending NUMBER packets\n");
	printf("  -i, --interval=NUMBER\t\twait NUMBER seconds between sending each packet\n");
	printf("      --ttl=N\t\t\tspecidy N as time-to-live\n");
	printf("  -v, --verbose\t\t\tverbose output\n");
	printf("  -w, --timeout=N\t\tstop after N seconds\n");
	printf("  -W, --linger=N\t\tnumber of seconds to wait for response\n");
	printf("\n");
	printf("  -p, --pattern=PATTERN\t\tfill ICMP packet with given pattern\n");
	printf("  -s, --size=NUMBER\t\tsend NUMBER data octets\n");
	printf("\n");
	printf("  -?, --help\t\t\tgive this help message\n");
}
