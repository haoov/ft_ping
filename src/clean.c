/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 09:03:25 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/17 12:12:25 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"

extern struct ping	ping;

/* 
 * Clean all allocated memory and close all opened fd. Called on exit
 * */
void	clean_all()
{
	if (ping.sockfd)
	{
		close(ping.sockfd);
	}
	if (ping.icmp_pkt)
	{
		free(ping.icmp_pkt);
	}
}

