/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 09:00:11 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/11 21:51:20 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"

int	main(const int argc, const char **argv)
{
	struct ft_ping	ft_ping;

	if (argc != 2)
	{
		errx(EXIT_FAILURE, "%s", ERR_NO_HOST);
	}
	memset(&ft_ping, 0, sizeof(struct ft_ping));
	on_exit(clean_all, &ft_ping);
	ft_ping.hostname = argv[1];
	ft_ping.sockfd = create_socket();
	ft_ping.dst = resolve_hostanme(ft_ping.hostname);
	print_ping(ft_ping.hostname, ft_ping.dst);
	return EXIT_SUCCESS;
}

