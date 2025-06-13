/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 07:42:05 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/13 13:16:01 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//this should be after sleep
//usleep is in microsecs
//get time since last meal
//calculate think duration (time_think)
//Handle cases where the calculated duration is zero or negative.
//prevent exesive long thinking times
//Determine the end time for the thinking phase.
//Simulate thinking time with a responsive loop.
void	philo_think(t_philo *philo)
{
	long long		think_ends;
	long long		time_think;
	long long		since_last_meal;
	int				sim_status;

	print_status(philo, "is thinking");
	pthread_mutex_lock(&philo->philo_mutex);
	since_last_meal = precise_time_ms() - philo->last_meal;
	pthread_mutex_unlock(&philo->philo_mutex);
	time_think = (philo->program->time_die - since_last_meal - 
			philo->program->time_eat) / 2;
	if (time_think <= 0)
		time_think = 1;
	else if (time_think > philo->program->time_eat)
		time_think = philo->program->time_eat;//new CHECK
	think_ends = precise_time_ms() + time_think;//PENDING MUTEX MAY BE
	while (precise_time_ms() < think_ends)
	{
		sim_status = check_end_cond(philo);
		if (sim_status == PHILO_DIED)
			return ;
		// usleep(500);
		usleep(10);//new
	}
}

//this should be after release forks
//time_t is in <sys/time.h> for time values
void	philo_sleep(t_philo *philo)
{
	long long		wake_up;
	int				sim_status;

	wake_up = precise_time_ms() + philo->program->time_sleep;
	print_status(philo, "is sleeping");
	while (precise_time_ms() < wake_up)
	{
		sim_status = check_end_cond(philo);
		if (sim_status == PHILO_DIED)
			return ;
		usleep(100);
	}
}
//helper function
void	set_fork_status(t_philo *philo, t_fork *fork_ptr, int status)
{
	if (fork_ptr == philo->left_fork)
		philo->lfork_status = status;
	else
		philo->rfork_status = status;
}
//take first the one with the lower ID
//needs to track how many forks were taken to avoid error in threads
// static void	take_forks(t_philo *philo)//check if this and realease are static
static int	take_forks(t_philo *philo)//check if this and realease are static
{
	t_fork	*first_fork;//check type
	t_fork	*second_fork;//check type
	int		status;//new

	//set cond to avoid attempt to take forks
	status = SUCCESS;//new
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
	pthread_mutex_lock(&first_fork->fork_mutex);//try to take first fork
	// philo->forks_taken = 1;//new
	set_fork_status(philo, first_fork, TAKEN_FORK);
	print_status(philo, "has taken a fork");
	if (check_end_cond(philo) == PHILO_DIED)
	{
		pthread_mutex_unlock(&first_fork->fork_mutex); // Liberar el tenedor si la simulación ha terminado
		// philo->forks_taken = 0;//new reset
		set_fork_status(philo, first_fork, TAKEN_FORK);//new
		return (ERROR); // Salir de la función ajsutar bien la macro de salida
	}
	pthread_mutex_lock(&second_fork->fork_mutex);
	// philo->forks_taken = 2;//new
	set_fork_status(philo, first_fork, TAKEN_FORK);//new
	print_status(philo, "has taken a fork");//after this hsould i include another check_end_cond?
	return (SUCCESS);
}

//only to mutex unlock
// Release in reverse order of acquisition
void	release_forks(t_philo *philo)//no message check and check if order is relevant
{
	t_fork	*first_fork;
	t_fork	*second_fork;

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
	if (philo->rfork_status == TAKEN_FORK)//new
	{
		pthread_mutex_unlock(&second_fork->fork_mutex);
		philo->rfork_status = NO_TAKEN;
	}
	if (philo->lfork_status == TAKEN_FORK)
	{
		pthread_mutex_unlock(&first_fork->fork_mutex);
		philo->lfork_status = NO_TAKEN;
	}
}

//before update last_meal check_end condition
//before update last meal lock mutex
//after uptdate last meal unlock mutex
//relasease fork after eating
void	philo_eat(t_philo *philo)
{
	long long	eat_end_time;
	int			sim_status;

	// take_forks(philo);
	if (take_forks(philo) == ERROR)//new check ajustar macro porque tal vez deberia ser FAILURE no es en si un error
		return ;//new check
	sim_status = check_end_cond(philo);
	if (sim_status == PHILO_DIED)//may be needs to release only 1 and not 2 depends how many has grabbed, primero debe ser liberado los forks por el que murio
	{
		release_forks(philo);
		return ;
	}
	pthread_mutex_lock(&philo->philo_mutex);
	philo->last_meal = precise_time_ms();
	philo->meal_number++;
	pthread_mutex_unlock(&philo->philo_mutex);
	print_status(philo, "is eating");
	eat_end_time = precise_time_ms () + philo->program->time_eat;
	while (precise_time_ms() < eat_end_time)
	{
		sim_status = check_end_cond(philo);
		if (sim_status == PHILO_DIED)
			break ;
		usleep(100);
	}
	release_forks(philo);
	// philo->forks_taken = 0;//new
}
