/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 09:03:25 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/11 09:12:57 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"

/* 
 * Clean all allocated memory and close all opened fd. Called on exit
 * */
void	clean_all(int status, void *data)
{
	struct ft_ping	*ft_ping;

	(void)status;
	ft_ping = (struct ft_ping*)data;
	if (ft_ping->sockfd)
	{
		shutdown(ft_ping->sockfd, SHUT_RDWR);
		close(ft_ping->sockfd);
	}
}

