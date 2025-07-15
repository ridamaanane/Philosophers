/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmaanane <rmaanane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 12:25:47 by rmaanane          #+#    #+#             */
/*   Updated: 2025/07/14 12:25:49 by rmaanane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	print_error(char *msg)
{
	write(2, msg, ft_strlen(msg));
	return (1);
}

unsigned int	ft_atoi(char *str)
{
	unsigned long	res;
	int				i;

	i = 0;
	res = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		if (res >= INT_MAX)
			return (INT_MAX);
		i++;
	}
	if (str[i])
		return (INT_MAX);
	return (res);
}
