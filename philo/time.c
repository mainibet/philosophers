/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:47:19 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/02 15:30:27 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// int set_initial_timestamps(t_program *ata)//new
// {
//     int i;

//     data->start_time = precise_time_ms();
//     if ((data->start_time == 0) // Basic error check for time function (rare)
//     {
//         print_error_msg("Failed to get start time.");
//         return (ERROR); // A generic error, or define a specific one if time functions can fail
//     }

//     i = 0;
//     while (i < data->total_philo)
//     {
//         data->philo[i].last_meal = data->start_time;
//         i++;
//     }
//     return (SUCCESS);
// }

//takes the current hour
//makes the equivalent to the starting moment I set
//make it precise milisec
//gettimeofday is very precise microsec
//* 1000LL convert sec to milisec
// /1000 convert microsec to milisec
//tv.tv_sec = seconds passed utils the moment is called
//tv.tv_usec =  microsec that additional to the seconds have passed
//conver microsec to milisec (tv.tv_usec / 1000)
long long	precise_time_ms(void)//NEW
{
	struct timeval	tv;
	long long		all_sec;
	long long		rem_microsec;
	long long		total_milisec;

	gettimeofday(&tv, NULL); // No timezone
	all_sec = tv.tv_sec * 1000LL;
	rem_microsec = tv.tv_usec / 1000;
	total_milisec = all_sec + rem_microsec;
	return (total_milisec);
}

//this should be after sleep
void	philo_think(t_philo *philo)
{
	long long	current_time;//check the type

    current_time = precise_time_ms () - philo->program->start_time;//check why atart time

    pthread_mutex_lock(&philo->program->output_mutex);
    printf("%lld %d is thinking\n", current_time, philo->philo_id);
    pthread_mutex_unlock(&philo->program->output_mutex);
    usleep(100000);//sleep for 100 ms testig
}
//this should be after release forks
void	philo_sleep(t_philo *philo)
{
	long long	wake_up;//check type of variables
	long long	current_time;//check type of variables

	pthread_mutex_lock(&philo->program->output_mutex);
	printf("%lld time_to_sleep received\n", philo->program->time_sleep);//check
	current_time = precise_time_ms() - philo->program->start_time;//update current time
	pthread_mutex_unlock(&philo->program->output_mutex);//test
    pthread_mutex_lock(&philo->program->output_mutex);//test
	printf("%lld %d is sleeping\n", current_time, philo->philo_id);//test
	pthread_mutex_unlock(&philo->program->output_mutex);//test
	current_time = precise_time_ms ();
	wake_up = current_time + philo->program->time_sleep;//check
	while (precise_time_ms() < wake_up)
		usleep(100);//check what # works
}

//each will be a status
//eating, thinking, and sleeping.
//prototype given by pthread_create
//void *(*start_routine)(void *), void *arg
//thread logic
void	*life_cycle(void *arg)
{
	t_philo		*philo;//check if can be use this struct
	long long	current_time;
	int			i;

	philo = (t_philo *)arg;
	current_time = precise_time_ms() - philo->program->start_time;
	printf("Philosopher %d is alive!\n", philo->philo_id);//test life cycle
	pthread_mutex_unlock(&philo->program->output_mutex);  //to unlock
    //pending to set a while loop with a cond to stop the loop term condition
	// take_forks(philo);//function with pthread_mutex_lock. 
    //check how to indicate take right and left if is odd or even
	i = 0;
	while (i < philo->program->total_philo)
	{
		// pthread_mutex_lock(&philo->program->output_mutex);//connect good data in philo before printing
		philo_sleep(philo);
		current_time = precise_time_ms() - philo->program->start_time;//update current time
		// printf("%lld %d time_to_sleep\n", current_time, philo->philo_id);//new to sleep
		pthread_mutex_unlock(&philo->program->output_mutex);
        current_time = precise_time_ms() - philo->program->start_time;//update current time
		philo_think(philo);
		i++;
	}
	printf("Philosopher %d is done.\n", philo->philo_id);//test only thread creation
	// eat(philo);//reacord time of last meal usleep(time_to_eat)
	// release_fork(philo); ///function to unlock mutex needs to be per each fork
	// sleep(philo); //usleep(time_to_sleep)
	// think(philo);//dynamic how should be set?
	return (NULL);
}
