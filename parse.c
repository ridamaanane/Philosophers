#include "philo.h"

int ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	print_error(char *msg)
{
	write(2, msg, ft_strlen(msg));
	return (1);          // return 1 to indicate failure
}

void	ft_usleep(time_t milliseconds, t_philos *philo) //target howa hadi kachecke kola mra yakma mat chi wahed bach thbs usleep katb9a khdama dima!! dakchi 3lach madrnahach
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


unsigned int    ft_atoi(char *str)
{
    unsigned long   res; //nrje3 lhad unisgned nfhmha
    int i;

    i = 0;
    res = 0;
    while (str[i] >= '0' && str[i] <= '9')
    {
        res = res * 10 + (str[i] - '0');
        if (res >= INT_MAX)///////////// rje3 lhadi darori !!!!!!!
            return (INT_MAX);
        i++;
    }
    if (str[i]) //ila kant ba9a chi haja fstring mn ghir ar9am  -->overflow wla invalid input
        return (INT_MAX); 
    return (res);
}

time_t	get_time(void)
{

	//(1 second = 1,000,000 microseconds)
	struct timeval	tv; //hiya struct li kat3mrha b  tv sec otv usec

	//tv mean cointener bach n7to fih lwa9t actual

	gettimeofday(&tv, NULL); //NULL meaning “Mahtajch timezone, tkhdem ghir b waqt local li 3nd OS”
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000)); //seconds → milliseconds || microseconds → milliseconds = total time in milliseconds

	//tv.tv_usec number of seconds men 1 Janvier 1970 (howa time fach bda unix)
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
	pthread_mutex_unlock(&philo->data->stop_mtx); //tb9a habta 7it kaynin des cas fihom mmchkil 
}

void *routine(void *arg) // routine kaybdel
{
	t_philos *philo = (t_philos *)arg; //3la hsab pthread create kat9bl void* argument dakchi 3lach drna haka 3ad castlina
	
	while (1)
	{
		pthread_mutex_lock(&philo->data->stop_mtx);
		if (!philo->data->stop_flag)// mean routine ysali
		{
			pthread_mutex_unlock(&philo->data->stop_mtx);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->data->stop_mtx);
		if (philo->id % 2 == 0) //even ( Bash ntfadow DEADLOCK!)
		{
			pthread_mutex_lock(philo->left_fork);
			log_action(philo, "has taken a fork");
			pthread_mutex_lock(philo->right_fork);
			log_action(philo, "has taken a fork");
		}
		else //odd
		{
			pthread_mutex_lock(philo->right_fork);
			log_action(philo, "has taken a fork");
			pthread_mutex_lock(philo->left_fork);
			log_action(philo, "has taken a fork");
		}
		pthread_mutex_lock(&philo->mtx_meal); //ma ykounch conflict ma3 monitor() li kay9raha
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
		if (philo->data->num_philosophers % 2 != 0) //drna odd 7it homa fach kayen cofnlit ama lakan even kaykon balance
			usleep(500);
	}


	return (NULL); //NULL f void * mean "no pointer" / empty address. //n9dro nrj3o ayy haja bghina ghir 3la hsab 7na hna mabghina walo drna NULL
}

//➡️ Function monitor katmonitori kol philosophers bach:

// wahed mat?

// kolshi klaw lgoal?

void mounitor(t_philos *philos) //mounitor kay9ra
{
	int i = 0;
	int num_of_philos;
	
	num_of_philos = philos->data->num_philosophers; // Variable bash n7fdo fih data.num_philosophers (bach manmchiwloch ldata kol waqt)

	while (1)
	{
		i = 0;
		while (i < num_of_philos)
		{
			pthread_mutex_lock(&philos[i].mtx_meal); //Sddina access 3la last_meal_time 7it bghina n9rawha safely
			if (get_time() - philos[i].last_meal_time > philos->data->time_to_die) //check ila philo mat bjou3
			{
				pthread_mutex_unlock(&philos[i].mtx_meal);
				pthread_mutex_lock(&philos[i].data->stop_mtx);
				philos->data->stop_flag = 0;
				pthread_mutex_unlock(&philos[i].data->stop_mtx);
				printf("%ld %d died\n", get_time() - philos->data->start_time, philos[i].id);
				return ;
			}
			pthread_mutex_unlock(&philos[i].mtx_meal); //ila madkhelch l if y3awd y7el
			i++;
		}
		if (philos->data->meal_goal > 0) //ila meal goal khawya makaydkhlch lhna 7itach ma3arfinch 3addad dmeals 3lach an7bso
		{
			pthread_mutex_lock(&philos->data->full_mtx); //Katsdd lmutex li kay7mi full_philo_count ---> 7it hadi shared variable: threads bzaf kayzido fih f nafs lwa9t
			if (philos->data->full_philo_count >= philos->data->num_philosophers)
			{
				pthread_mutex_unlock(&philos->data->full_mtx);
				pthread_mutex_lock(&philos->data->stop_mtx);
				philos->data->stop_flag = 0;
				pthread_mutex_unlock(&philos->data->stop_mtx);
				return ;
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

	if (data.num_philosophers <= 0 || data.num_philosophers > 200)
		return (print_error("Error: invalid number of philosophers\n"));
	if (data.time_to_die == INT_MAX || data.time_to_eat == INT_MAX || data.time_to_sleep == INT_MAX)
		return (print_error("Error: invalid number of philosophers\n"));


	data.meal_goal = -1; //makaynach doc simulation ghadi tsali ghir ila chi wahed mat
	if (ac == 6)
		data.meal_goal = ft_atoi(av[5]);

	
	pthread_mutex_init(&data.stop_mtx, NULL); // Katinitializi mutex li kay7mi stop_flag
	pthread_mutex_init(&data.full_mtx, NULL); //Katinitializi mutex li kay7mi full_philo_count
	data.full_philo_count = 0;  //madrnach mutex init 7it howa aslan int fstruct may7tajch

	i = 0;
	while (i < data.num_philosophers)
		pthread_mutex_init(&data.forks[i++], NULL); //initialisation 3la hsab kol philisopher

	i = 0;
	while (i < data.num_philosophers)
	{
		philos[i].id = i + 1;
		philos[i].data = &data; //bach kanchofo config dkola philo mn struct t_data
		philos[i].left_fork = &data.forks[i];
		philos[i].right_fork = &data.forks [(i + 1) % data.num_philosophers];  //matalan 5 philo dak lkher howa 5 bdina hsab b 1 --> 5 + 1 % 5 = 1 y3ni fork right dyalo hiya 1 
		philos[i].last_meal_time = get_time();
		philos[i].meal_count = 0; //7it hna mazal makla walo
		pthread_mutex_init(&philos[i].mtx_meal, NULL); //NUll mean  katsgoul lmutex ytsayb b default config (normal mutex, standard behavior)
		i++;
	}

	data.start_time = get_time();
	data.stop_flag = 1; //7it kancheckiw mn b3d if(stop flag) ila l9a wahed kaykmel 0 kayhbs

	i = 1; //even (katzad 3lih 1 so kaywli 2)
	while (i < data.num_philosophers)
	{
		pthread_create(&data.threads[i] , NULL, routine, &philos[i]);
		i += 2;
	}
	usleep(500);  //0.5 millisecond
	i = 0; //odd (katzad 3lih 1 so kaywli 1))
	while (i < data.num_philosophers)
	{
		pthread_create(&data.threads[i] , NULL, routine, &philos[i]); // &data.thread[i] (id) ,  &philos[i] bach t3rf routine ina philo khdamin 3lih (info 3lih fstruct (id, forks, last..))
		i += 2;
	}

	mounitor(philos);

}
