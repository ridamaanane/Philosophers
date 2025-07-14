/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmaanane <rmaanane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 12:26:08 by rmaanane          #+#    #+#             */
/*   Updated: 2025/07/14 16:03:26 by rmaanane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philos *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		log_action(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		log_action(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		log_action(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		log_action(philo, "has taken a fork");
	}
}

void	eat(t_philos *philo)
{
	pthread_mutex_lock(&philo->mtx_meal);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->mtx_meal);
	log_action(philo, "is eating");
	philo->meal_count++;
	if (philo->meal_count == philo->data->meal_goal)
	{
		pthread_mutex_lock(&philo->data->full_mtx);
		philo->data->full_philo_count++;
		pthread_mutex_unlock(&philo->data->full_mtx);
	}
	smart_usleep(philo->data->time_to_eat, philo);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

int	handle_one_philo(t_philos *philos)
{
	if (philos->data->num_philosophers == 1)
	{
		log_action(philos, "has taken a fork");
		smart_usleep(philos->data->time_to_die, philos);
		return (1);
	}
	return (0);
}

void	*routine(void *arg)
{
	t_philos	*philo;

	philo = (t_philos *)arg;
	if (handle_one_philo(philo))
		return (NULL);
	while (1)
	{
		pthread_mutex_lock(&philo->data->stop_mtx);
		if (!philo->data->stop_flag)
		{
			pthread_mutex_unlock(&philo->data->stop_mtx);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->data->stop_mtx);
		take_forks(philo);
		eat(philo);
		log_action(philo, "is sleeping");
		smart_usleep(philo->data->time_to_sleep, philo);
		log_action(philo, "is thinking");
		if (philo->data->num_philosophers % 2 != 0)
			usleep(500);
	}
	return (NULL);
}
