#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_data
{
	int				num_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meal_goal;
	int				stop_flag;
	time_t			start_time;
	pthread_mutex_t	forks[200];
	pthread_t		threads[200];
}	t_data;

typedef struct s_philos
{
	int				id;
	int				times_eaten;
	time_t			last_meal_time;
	t_data			*data;
	pthread_mutex_t	left_fork;
	pthread_mutex_t	right_fork;
}	t_philos;

#endif
