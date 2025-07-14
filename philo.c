/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmaanane <rmaanane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 12:25:54 by rmaanane          #+#    #+#             */
/*   Updated: 2025/07/14 18:07:45 by rmaanane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	log_action(t_philos *philo, char *action)
{
	pthread_mutex_lock(&philo->data->stop_mtx);
	if (!philo->data->stop_flag)
	{
		pthread_mutex_unlock(&philo->data->stop_mtx);
		return ;
	}
	pthread_mutex_lock(&philo->data->print_mtx);
	printf("%ld %d %s\n", get_time() - philo->data->start_time, philo->id,
		action);
	pthread_mutex_ulock(&philo->data->print_mtx);
	pthread_mutex_unlock(&philo->data->stop_mtx);
}

void	init_philosophers(t_data *data, t_philos *philos)
{
	int	i;

	i = 0;
	while (i < data->num_philosophers)
	{
		philos[i].id = i + 1;
		philos[i].data = data;
		philos[i].left_fork = &data->forks[i];
		philos[i].right_fork = &data->forks[(i + 1) % data->num_philosophers];
		philos[i].last_meal_time = get_time();
		philos[i].meal_count = 0;
		pthread_mutex_init(&philos[i].mtx_meal, NULL);
		i++;
	}
}

void	init_mutexes(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_init(&data->print_mtx, NULL);
	pthread_mutex_init(&data->stop_mtx, NULL);
	pthread_mutex_init(&data->full_mtx, NULL);
	data->full_philo_count = 0;
	while (i < data->num_philosophers)
		pthread_mutex_init(&data->forks[i++], NULL);
}

void	start_simulation(t_data *data, t_philos *philos)
{
	int	i;

	i = 1;
	data->start_time = get_time();
	data->stop_flag = 1;
	while (i < data->num_philosophers)
	{
		pthread_create(&data->threads[i], NULL, routine, &philos[i]);
		i += 2;
	}
	usleep(500);
	i = 0;
	while (i < data->num_philosophers)
	{
		pthread_create(&data->threads[i], NULL, routine, &philos[i]);
		i += 2;
	}
	mounitor(philos);
}

int	main(int ac, char **av)
{
	t_philos	philos[200];
	t_data		data;
	int			i;

	if (ac != 5 && ac != 6)
		return (1);
	data.num_philosophers = ft_atoi(av[1]);
	data.time_to_die = ft_atoi(av[2]);
	data.time_to_eat = ft_atoi(av[3]);
	data.time_to_sleep = ft_atoi(av[4]);
	data.meal_goal = -1;
	if (ac == 6)
		data.meal_goal = ft_atoi(av[5]);
	if (check_error(data))
		return (1);
	init_mutexes(&data);
	init_philosophers(&data, philos);
	data.start_time = get_time();
	data.stop_flag = 1;
	start_simulation(&data, philos);
	i = 0;
	while (i < data.num_philosophers)
		pthread_join(data.threads[i++], NULL);
	cleaning_mutexes(philos, &data);
}
