#include "philo.h"

time_t	get_time(void)
{

	//(1 second = 1,000,000 microseconds)
	struct timeval	tv; //hiya struct li kat3mrha b  tv sec otv usec

	//tv mean cointener bach n7to fih lwa9t actual

	gettimeofday(&tv, NULL); //NULL meaning “Mahtajch timezone, tkhdem ghir b waqt local li 3nd OS”
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000)); //seconds → milliseconds || microseconds → milliseconds = total time in milliseconds

	//tv.tv_usec number of seconds men 1 Janvier 1970 (howa time fach bda unix)
}

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
		usleep(500);
	}
	
}

void    cleaning_mutexes(t_philos *philos, t_data *data)
{
		int     i;

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