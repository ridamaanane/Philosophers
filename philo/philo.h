/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmaanane <rmaanane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 12:26:02 by rmaanane          #+#    #+#             */
/*   Updated: 2025/07/15 15:48:58 by rmaanane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
//# include <stdlib.h>
// # include <string.h>
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
	pthread_mutex_t	stop_mtx;
	pthread_mutex_t	full_mtx;
	int				full_philo_count;
	pthread_mutex_t	print_mtx;
}					t_data;

typedef struct s_philos
{
	int				id;
	int				times_eaten;
	int				meal_count;
	time_t			last_meal_time;
	t_data			*data;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	mtx_meal;
}					t_philos;

// routine
void				*routine(void *arg);

// mounitor
void				mounitor(t_philos *philos);

// parce
void				log_action(t_philos *philo, char *action);
void				init_philosophers(t_data *data, t_philos *philos);
void				init_mutexes(t_data *data);
void				start_simulation(t_data *data, t_philos *philos);

// utils
time_t				get_time(void);
void				smart_usleep(time_t milliseconds, t_philos *philo);
int					check_error(t_data data);
void				cleaning_mutexes(t_philos *philos, t_data *data);

// parse
unsigned int		ft_atoi(char *str);
int					print_error(char *msg);

#endif
