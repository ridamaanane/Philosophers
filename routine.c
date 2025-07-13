
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
	ft_usleep(philo->data->time_to_eat, philo);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void *routine(void *arg)
{
	t_philos *philo = (t_philos *)arg;
	
	if (philo->data->num_philosophers == 1)
	{
		log_action(philo, "has taken a fork");
		ft_usleep(philo->data->time_to_die, philo);
		return (NULL);
	}
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
		ft_usleep(philo->data->time_to_sleep, philo);
		log_action(philo, "is thinking");
		if (philo->data->num_philosophers % 2 != 0) 
			usleep(500);
	}
	return (NULL); 
}
