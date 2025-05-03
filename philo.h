#ifndef PHILO_H
#define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <limits.h>


#define MAX_PHILOS 200

typedef struct s_data
{
    int num_philosophers;
    time_t start_time;
    time_t time_to_die; //time imta ghadi ymot mn akhir mra kla
    time_t time_to_eat;
    time_t time_to_sleep;
    int meal_goal;
    pthread_mutex_t forks[MAX_PHILOS];
    int stop_flag;
}   t_data;


typedef struct s_philos
{
    pthread_t id;
    int times_eaten;	
    time_t last_meal_time;	
    pthread_mutex_t left_fork;
    pthread_mutex_t right_fork;
    t_data *data;
    pthread_t thread;
}   t_philos;


#endif