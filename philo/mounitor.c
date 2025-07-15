/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mounitor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmaanane <rmaanane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 12:25:37 by rmaanane          #+#    #+#             */
/*   Updated: 2025/07/14 17:48:01 by rmaanane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_deaths(t_philos *philos, int num)
{
	int	i;

	i = 0;
	while (i < num)
	{
		pthread_mutex_lock(&philos[i].mtx_meal);
		if (get_time() - philos[i].last_meal_time > philos->data->time_to_die)
		{
			pthread_mutex_unlock(&philos[i].mtx_meal);
			pthread_mutex_lock(&philos[i].data->stop_mtx);
			philos->data->stop_flag = 0;
			pthread_mutex_unlock(&philos[i].data->stop_mtx);
			pthread_mutex_lock(&philos[i].data->print_mtx);
			printf("%ld %d died\n", get_time() - philos->data->start_time,
				philos[i].id);
			pthread_mutex_unlock(&philos[i].data->print_mtx);
			return (1);
		}
		pthread_mutex_unlock(&philos[i].mtx_meal);
		i++;
	}
	return (0);
}

int	check_meal_goal(t_philos *philos)
{
	if (philos->data->meal_goal > 0)
	{
		pthread_mutex_lock(&philos->data->full_mtx);
		if (philos->data->full_philo_count >= philos->data->num_philosophers)
		{
			pthread_mutex_unlock(&philos->data->full_mtx);
			pthread_mutex_lock(&philos->data->stop_mtx);
			philos->data->stop_flag = 0;
			pthread_mutex_unlock(&philos->data->stop_mtx);
			return (1);
		}
		pthread_mutex_unlock(&philos->data->full_mtx);
	}
	return (0);
}

void	mounitor(t_philos *philos)
{
	int	num_of_philos;

	num_of_philos = philos->data->num_philosophers;
	while (1)
	{
		if (check_deaths(philos, num_of_philos))
			return ;
		if (check_meal_goal(philos))
			return ;
		usleep(500);
	}
}
