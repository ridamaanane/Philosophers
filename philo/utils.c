/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmaanane <rmaanane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 12:26:14 by rmaanane          #+#    #+#             */
/*   Updated: 2025/07/15 15:51:28 by rmaanane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

time_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	smart_usleep(time_t milliseconds, t_philos *philo)
{
	time_t	start;

	start = get_time();
	while ((get_time() - start) < milliseconds)
	{
		pthread_mutex_lock(&philo->data->stop_mtx);
		if (!philo->data->stop_flag)
		{
			pthread_mutex_unlock(&philo->data->stop_mtx);
			return ;
		}
		pthread_mutex_unlock(&philo->data->stop_mtx);
		usleep(500);
	}
}

int	check_error(t_data data)
{
	if (data.num_philosophers <= 0 || data.num_philosophers > 200)
		return (print_error("Error: invalid number of philosophers\n"));
	if (data.time_to_die == INT_MAX || data.time_to_eat == INT_MAX
		|| data.time_to_sleep == INT_MAX || data.meal_goal == INT_MAX)
		return (print_error("Error: invalid number of philosophers\n"));
	if (data.meal_goal == 0)
		return (print_error("Error: Can't eat 0 meal\n"));
	return (0);
}

void	cleaning_mutexes(t_philos *philos, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philosophers)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&philos[i].mtx_meal);
		i++;
	}
	pthread_mutex_destroy(&data->stop_mtx);
	pthread_mutex_destroy(&data->full_mtx);
}
