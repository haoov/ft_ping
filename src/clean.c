/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 09:03:25 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/12 18:09:41 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"

/* 
 * Clean all allocated memory and close all opened fd. Called on exit
 * */
void clean_all(int status, void *data) {
	struct ft_ping	*ft_ping;

	ft_ping = (struct ft_ping*)data;
	if (status == EXIT_SUCCESS) {
		print_stats(ft_ping);
	}
	if (ft_ping->sockfd) {
		close(ft_ping->sockfd);
	}
	if (ft_ping->icmp_pkt) {
		free(ft_ping->icmp_pkt);
	}
}

