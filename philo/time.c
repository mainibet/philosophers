/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:47:19 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/03 08:46:16 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//HELPER FUNCTION
//to print all the msg in order
void	print_status(t_philo *philo, const char *msg)
{
	long long	current_time;

	current_time = precise_time_ms() - philo->program->start_time;
	pthread_mutex_lock(&philo->program->output_mutex);//after this includ checks of termination conditions with a flag
    //also the specific printing to finish lock and unlock
	printf("%lld %d %s\n", current_time, philo->philo_id, msg);
	pthread_mutex_unlock(&philo->program->output_mutex);
}

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
// No timezone
long long	precise_time_ms(void)
{
	struct timeval	tv;
	long long		all_sec;
	long long		rem_microsec;
	long long		total_milisec;

	gettimeofday(&tv, NULL); 
	all_sec = tv.tv_sec * 1000LL;
	rem_microsec = tv.tv_usec / 1000;
	total_milisec = all_sec + rem_microsec;
	return (total_milisec);
}

//this should be after sleep
//usleep is in microsecs
void	philo_think(t_philo *philo)
{
	time_t		current_time;
	const char	*msg;

	current_time = precise_time_ms () - philo->program->start_time;//check why atart time
	msg = "is thinking";
//set if cond to stop if terminates condition reach
	print_status(philo, msg);
	usleep(100);
}
//this should be after release forks
//time_t is in <sys/time.h> for time values
void	philo_sleep(t_philo *philo)
{
	time_t		wake_up;
	// time_t		current_time;
	const char	*msg;

	wake_up = philo->program->start_time + philo->program->time_sleep;//check
	// current_time = precise_time_ms () - philo->program->start_time;//check may be can be defined in the life_cycle
	msg = "is sleeping";
	print_status(philo, msg);
	while (precise_time_ms() < wake_up)
	{
		//condition to stop in termination cond
		usleep(100);
	}
}

//take first the one with the lower ID
void	take_forks(t_philo *philo)
{
	t_fork	*first_fork;//check type
	t_fork	*second_fork;//check type
	const char	* msg;

	//set cond to finish if terminates condition reached
	msg = "has taken a fork";
	if (philo->left_fork->fork_id < philo->right_fork->fork_id)
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	else
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	// if (pthread_mutex_lock(&first_fork->mutex) != 0) //handling error already lock
	// 	return 	;
	// print_status(philo, msg);
	// if (pthread_mutex_lock(&second_fork->mutex) != 0)//handling error already lock
	// {
	// 	pthread_mutex_unlock(&first_fork->mutex);  // Release first fork if can't get second
	// 	return ;
	// }
	// print_status(philo, msg);
	pthread_mutex_lock(&first_fork->mutex);
	print_status(philo, msg);
	pthread_mutex_lock(&second_fork->mutex);
	print_status(philo, msg);
}
//only to mutex unlock
//unlock in reverse order
void	release_forks(t_philo *philo)//no message check
{
	printf("forks about to be realesed\n");//test
	if (philo->left_fork->fork_id < philo->right_fork->fork_id)//is it relevant the order?
	{
		pthread_mutex_unlock(&philo->right_fork->mutex);
		pthread_mutex_unlock(&philo->left_fork->mutex);
	}
	else
	{
		pthread_mutex_unlock(&philo->left_fork->mutex);
		pthread_mutex_unlock(&philo->right_fork->mutex);
	}
	printf("forks realesed\n");//test
}

void	philo_eat(t_philo *philo)
{
    usleep(100);//test
	take_forks(philo);
	release_forks(philo);
}

//each will be a status
//eating, thinking, and sleeping.
//prototype given by pthread_create
//void *(*start_routine)(void *), void *arg
//thread logic
void	*life_cycle(void *arg)
{
	t_philo	*philo;//check if can be use this struct
	time_t	current_time;
	int		i;

	philo = (t_philo *)arg;
	current_time = precise_time_ms() - philo->program->start_time;
    //pending to set a while loop with a cond to stop the loop term condition
	// take_forks(philo);//function with pthread_mutex_lock. 
    //check how to indicate take right and left if is odd or even
	i = 0;
	while (i < philo->program->total_philo)
	{
		philo_sleep(philo);
		current_time = precise_time_ms() - philo->program->start_time;//update current time
		philo_think(philo);
		philo_eat(philo);//test
		i++;
	}
	printf("Philosopher %d is done.\n", philo->philo_id);//test only thread creation
	// eat(philo);//reacord time of last meal usleep(time_to_eat)
	// release_fork(philo); ///function to unlock mutex needs to be per each fork
	// sleep(philo); //usleep(time_to_sleep)
	// think(philo);//dynamic how should be set?
	return (NULL);
}
