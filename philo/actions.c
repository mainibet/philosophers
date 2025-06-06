/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 07:42:05 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/06 07:47:51 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//this should be after sleep
//usleep is in microsecs
void	philo_think(t_philo *philo)
{
	long long		current_time;
	long long		think_time;

	current_time = precise_time_ms () - philo->program->start_time;//check why atart time
//set if cond to stop if terminates condition reach
	print_status(philo, "is thinking");
	// usleep(100);
    // Calculate think time: use time between eat and die to prevent death
	think_time = (philo->program->time_die - philo->program->time_eat - philo->program->time_sleep) / 2;//check
	if (think_time > 0)
		usleep(think_time * 1000);
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
		if (sim_status)//check if is ok
			return ;
		usleep(100);
	}
}

//take first the one with the lower ID
static void	take_forks(t_philo *philo)//check if this and realease are static
{
	t_fork	*first_fork;//check type
	t_fork	*second_fork;//check type

	//set cond to avoid attempt to take forks
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
	pthread_mutex_lock(&first_fork->mutex);
	print_status(philo, "has taken a fork");
    //check if simulation hasn't stop
	pthread_mutex_lock(&second_fork->mutex);
	print_status(philo, "has taken a fork");
}

//only to mutex unlock
static void	release_forks(t_philo *philo)//no message check and check if order is relevant
{
	t_fork	*first_fork;
	t_fork	*second_fork;

	// same order logic as take_forks
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
    // Release in reverse order of acquisition
	pthread_mutex_unlock(&second_fork->mutex);
	pthread_mutex_unlock(&first_fork->mutex);
}

//before update last_meal check_end condition
//before update last meal lock mutex
//after uptdate last meal unlock mutex
//relasease fork after eating
void	philo_eat(t_philo *philo)
{
	long long	eat_end_time;
	int			sim_status;

	take_forks(philo);
	sim_status = check_end_cond(philo);
	if (sim_status == PHILO_DIED)
	{
		release_forks(philo);
		return ;
	}
	pthread_mutex_lock(&philo->philo_mutex);//new
	philo->last_meal = precise_time_ms();
	philo->meal_number++;
	pthread_mutex_unlock(&philo->philo_mutex);
	print_status(philo, "is eating");
	eat_end_time = precise_time_ms () + philo->program->time_eat;
	while (precise_time_ms() < eat_end_time)
	{
		sim_status = check_end_cond(philo);
		if (sim_status == PHILO_DIED)
			break;
		//set if condition if simulation stops
		usleep(100);//test
	}
	release_forks(philo);
}
