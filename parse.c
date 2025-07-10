#include "philo.h"


time_t	get_time(void);

void	ft_usleep(time_t milliseconds, t_philos *philo)
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
		usleep(500); //half of a millisec
	}
	
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
	{
		result = result * 10 + (str[i++] - '0');
		if (result > INT_MAX)
		{
			printf("overflow");
			exit(1);
		}
	}
	return (sign * result);
}

time_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void log_action(t_philos *philo, char *action)
{
	pthread_mutex_lock(&philo->data->stop_mtx);
	if (!philo->data->stop_flag)
	{
		pthread_mutex_unlock(&philo->data->stop_mtx);
		return ;
	}
	printf("%ld %d %s\n", get_time() - philo->data->start_time, philo->id, action);
	pthread_mutex_unlock(&philo->data->stop_mtx);
}

void *routine(void *arg)
{
	t_philos *philo = (t_philos *)arg;
	
	while (1)
	{
		pthread_mutex_lock(&philo->data->stop_mtx);
		if (!philo->data->stop_flag)
		{
			pthread_mutex_unlock(&philo->data->stop_mtx);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->data->stop_mtx);
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
		ft_usleep(philo->data->time_to_eat, philo);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		log_action(philo, "is sleeping");
		ft_usleep(philo->data->time_to_sleep, philo);
		log_action(philo, "is thinking");
		if (philo->data->num_philosophers % 2 != 0)
			usleep(500);
	}
	return (NULL); //NULL f void * mean "no pointer" / empty address. //n9dro nrj3o ayy haja bghina ghir 3la hsab 7na hna mabghina walo drna NULL
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
			pthread_mutex_lock(&philos[i].mtx_meal);
			if (get_time() - philos[i].last_meal_time > philos->data->time_to_die)
			{
				pthread_mutex_lock(&philos[i].data->stop_mtx);
				philos->data->stop_flag = 0;
				pthread_mutex_unlock(&philos[i].data->stop_mtx);
				pthread_mutex_unlock(&philos[i].mtx_meal);
				printf("%ld %d died\n", get_time() - philos->data->start_time, philos[i].id);
				return (NULL);
			}
			pthread_mutex_unlock(&philos[i].mtx_meal);
			
			
			i++;
		}
		if (philos->data->meal_goal > 0)
		{
			pthread_mutex_lock(&philos->data->full_mtx);
			if (philos->data->full_philo_count >= philos->data->meal_goal)
			{
				pthread_mutex_unlock(&philos->data->full_mtx);
				return (NULL);
			}
			pthread_mutex_unlock(&philos->data->full_mtx);
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
	pthread_mutex_init(&data.stop_mtx, NULL);
	pthread_mutex_init(&data.full_mtx, NULL);
	data.full_philo_count = 0;
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
		philos[i].meal_count = 0;
		pthread_mutex_init(&philos[i].mtx_meal, NULL);
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
