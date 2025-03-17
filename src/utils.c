/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 12:05:48 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/17 12:13:05 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

double	reducef(double *arr, size_t size)
{
	double sum;

	sum = 0;
	for (size_t i = 0; arr[i] && i < size; ++i)
	{
		sum += arr[i];
	}
	return sum;
}

