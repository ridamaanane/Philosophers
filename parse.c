#include "philo.h"

int	ft_atoi(char *str)
{
	int	i = 0;
	int	result = 0;
	int	sign = 1;

	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
		result = result * 10 + (str[i++] - '0');
	return (sign * result);
}

time_t	curr_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	print_status(char *msg, t_philos *philo)
{
	if (philo->data->stop_flag)
		return ;
	printf("%ld %d %s\n", curr_time() - philo->data->start_time, philo->id, msg);
}

void	round_for_each(t_philos *philo)
{
	pthread_mutex_lock(&philo->left_fork);
	print_status("has taken a fork", philo);
	pthread_mutex_lock(&philo->right_fork);
	print_status("has taken a fork", philo);

	print_status("is eating", philo);
	philo->last_meal_time = curr_time();
	usleep(philo->data->time_to_eat * 1000);
	philo->times_eaten++;

	pthread_mutex_unlock(&philo->left_fork);
	pthread_mutex_unlock(&philo->right_fork);

	print_status("is sleeping", philo);
	usleep(philo->data->time_to_sleep * 1000);

	print_status("is thinking", philo);
}

void	*routine(void *arg)
{
	t_philos	*philo = (t_philos *)arg;

	if (philo->id % 2 == 0)
		usleep(1000);

	while (!philo->data->stop_flag)
	{
		round_for_each(philo);
	}
	return (NULL);
}

int	simulation_end(t_philos *philo)
{
	if ((curr_time() - philo->last_meal_time) >= philo->data->time_to_die)
	{
		philo->data->stop_flag = 1;
		printf("%ld %d died\n", curr_time() - philo->data->start_time, philo->id);
		return (1);
	}
	if (philo->data->meal_goal > 0 &&
		philo->times_eaten >= philo->data->meal_goal)
	{
		return (2);
	}
	return (0);
}

void	monitor(t_philos *philos)
{
	int	i;
	int	full;

	while (!philos[0].data->stop_flag)
	{
		i = 0;
		full = 0;
		while (i < philos[0].data->num_philosophers)
		{
			int result = simulation_end(&philos[i]);
			if (result == 1)
				return ;
			else if (result == 2)
				full++;
			i++;
		}
		if (philos[0].data->meal_goal > 0 &&
			full == philos[0].data->num_philosophers)
		{
			philos[0].data->stop_flag = 1;
			return ;
		}
		usleep(1000);
	}
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
	data.stop_flag = 0;
	data.meal_goal = -1;
	if (ac == 6)
		data.meal_goal = ft_atoi(av[5]);
		
	i = 0;
	while (i < data.num_philosophers)
		pthread_mutex_init(&data.forks[i++], NULL);
	i = 0;
	while (i < data.num_philosophers)
	{
		philos[i].id = i + 1;
		philos[i].left_fork = data.forks[i];
		philos[i].right_fork = data.forks[(i + 1) % data.num_philosophers];
		philos[i].data = &data;
		philos[i].times_eaten = 0;
		philos[i].last_meal_time = curr_time();
		i++;
	}
	data.start_time = curr_time();
	i = 0;
	while (i < data.num_philosophers)
	{
		pthread_create(&data.threads[i], NULL, routine, &philos[i]);
		i++;
	}

	monitor(philos);

	i = 0;
	while (i < data.num_philosophers)
	{
		pthread_join(data.threads[i], NULL);		
		i++;
	}
	i = 0;
	while (i < data.num_philosophers)
	{
		pthread_mutex_destroy(&data.forks[i]);
		i++;
	}
}
