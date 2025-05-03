#include "philo.h"

int	ft_atoi(char *str)
{
	int		i;
	int	result;
	int	sign;

	i = 0;
	result = 0;
	sign = 1;
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
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (sign * result);
}

time_t curr_time()
{
    struct timeval vl;

    gettimeofday(&vl, NULL);
    return (vl.tv_sec*1000 + vl.tv_usec / 1000);
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

}

