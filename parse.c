#include "philo.h"
time_t	get_time(void);
void	ft_usleep(time_t milliseconds, t_philos *philo)
{
	time_t	start;

	start = get_time();
	while ((get_time() - start) < milliseconds && philo->data->stop_flag)
		usleep(500);
}

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

time_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}


void *routine(void *arg)
{
	t_philos *philo = (t_philos *)arg;
	
	while (philo->data->stop_flag)
	{
		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(philo->left_fork);
			printf("%ld %d has taken a fork\n", get_time() - philo->data->start_time, philo->id);
			pthread_mutex_lock(philo->right_fork);
			printf("%ld %d has taken a fork\n", get_time() - philo->data->start_time, philo->id);
		}
		else
		{
			pthread_mutex_lock(philo->right_fork);
			printf("%ld %d has taken a fork\n", get_time() - philo->data->start_time, philo->id);
			pthread_mutex_lock(philo->left_fork);
			printf("%ld %d has taken a fork\n", get_time() - philo->data->start_time, philo->id);
		}
		if (philo->data->stop_flag)
			printf("%ld %d has eating\n", get_time() - philo->data->start_time, philo->id);
		philo->last_meal_time = get_time();
		ft_usleep(philo->data->time_to_eat, philo);
		
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		
		if (philo->data->stop_flag)
			printf("%ld %d is sleeping\n", get_time() - philo->data->start_time, philo->id);
		
		ft_usleep(philo->data->time_to_sleep, philo);
			
		if (philo->data->stop_flag)
			printf("%ld %d is thinking\n", get_time() - philo->data->start_time, philo->id);
	}
	

}

void *mounitor(t_philos *philos)
{
	int i = 0;
	int num_of_philos;
	
	num_of_philos = philos->data->num_philosophers;

	while (1)
	{
		i = 0;
		while (i < num_of_philos)
		{
			if (get_time() - philos[i].last_meal_time > philos->data->time_to_die)
			{
				printf("%ld %d died\n", get_time() - philos->data->start_time, philos[i].id);
				philos->data->stop_flag = 0;
				return (NULL);
			}
			i++;
		}
		usleep(500);
	}
}

int main(int ac, char **av)
{
	t_philos philos[200];
	t_data data;
	int i;

	if (ac != 5 && ac != 6)
		return (1);
	data.num_philosophers = ft_atoi(av[1]);
	data.time_to_die = ft_atoi(av[2]);
	data.time_to_eat = ft_atoi(av[3]);
	data.time_to_sleep = ft_atoi(av[4]);

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
		philos[i].data = &data;
		philos[i].left_fork = &data.forks[i];
		philos[i].right_fork = &data.forks [(i + 1) % data.num_philosophers];
		philos[i].last_meal_time = get_time();
		i++;
	}

	data.start_time = get_time();
	data.stop_flag = 1;

	i = 0;
	while (i < data.num_philosophers)
	{
		pthread_create(&data.threads[i] , NULL, routine, &philos[i]);
		i++;
	}

	mounitor(philos);

}
