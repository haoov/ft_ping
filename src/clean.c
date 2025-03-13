/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 09:03:25 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/13 12:20:42 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"

/* 
 * Clean all allocated memory and close all opened fd. Called on exit
 * */
void clean_all(int status, void *data) {
	struct ping	*ping;

	ping = (struct ping*)data;
	if (status == EXIT_SUCCESS) {
		ping_stats(ping);
	}
	if (ping->sockfd) {
		close(ping->sockfd);
	}
	if (ping->icmp_pkt) {
		free(ping->icmp_pkt);
	}
}

