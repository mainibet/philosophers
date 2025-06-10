/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_cycle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 07:49:31 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/10 09:50:52 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_routine_odd(t_philo *philo)
{
	philo_eat(philo);
	philo_sleep(philo);
	philo_think(philo);
}

static void	philo_routine_even(t_philo *philo)
{
	philo_think(philo);
	philo_eat(philo);
	philo_sleep(philo);
}

//each will be a status
//eating, thinking, and sleeping.
//prototype given by pthread_create
//void *(*start_routine)(void *), void *arg
//thread logic
//odd: eat-sleep-think
//even: think-eat-sleep
//After a full routine, check if max_meals is reached for this philo
// (The monitor will check if *all* philos have reached it)
// usleep (100) is to Yield CPU to avoid busy-waiting
//steps:
//2. Synchronization to wait for simulation start signal
//end syncronization
//Initialize last_meal and meal_number after synchronization
//2.  Initialize philosopher's state after global start time is set 
//This happens *after* all threads are ready and 'start_time' is reliable.
//It's essential for the monitor to accurately track hunger from the beginning.
//We lock the philo_mutex to safely update last_meal and meal_number.
//3. Initial delay for even-ID philosophers to prevent immediate deadlock
void	*life_cycle(void *arg)
{
	t_philo	*philo;
	// int		wait_time;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->program->start_mutex);//here the sync begun
	while (philo->program->sim_status == SIM_STOP)
	{
		pthread_mutex_unlock(&philo->program->start_mutex);
		usleep(100); 
		pthread_mutex_lock(&philo->program->start_mutex);
	}
	pthread_mutex_unlock(&philo->program->start_mutex);//here the sync ends
	// philo->last_meal = philo->program->start_time;//why would this work if is the very begining?
	pthread_mutex_lock(&philo->philo_mutex);//init philos
	philo->last_meal = philo->program->start_time; // Set initial last_meal to global simulation start time
	philo->meal_number = 0; // Ensure meal_number is 0 at the start of its life cycle
	pthread_mutex_unlock(&philo->philo_mutex);//end init philos
	if (philo->philo_id % 2 == 0)//init delay for even philos o prevent immediate deadlock
		usleep(100);
	while (1)
	{
		if (check_end_cond(philo) == PHILO_DIED)
			break ;
		if (philo->philo_id % 2 != 0)
			philo_routine_odd(philo);
		else
			philo_routine_even(philo);
		if (check_end_cond(philo) == PHILO_DIED)
			break ;
		pthread_mutex_lock(&philo->philo_mutex);//check if specific philo reached max meals
		if (philo->program->max_meals != MAX_MEALS_DISABLED
			&& philo->meal_number >= philo->program->max_meals)
		{
			pthread_mutex_unlock(&philo->philo_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->philo_mutex);
	}
	return (NULL);
}
