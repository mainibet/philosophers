/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_cycle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 07:49:31 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/16 16:36:46 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_routine_odd(t_philo *philo)
{
	if (check_end_cond(philo) == PHILO_DIED)
		return ;
	philo_eat(philo);
	if (check_end_cond(philo) == PHILO_DIED)
		return ;
	philo_sleep(philo);
	if (check_end_cond(philo) == PHILO_DIED)
		return ;
	philo_think(philo);
	if (check_end_cond(philo) == PHILO_DIED)
		return ;
}

static void	philo_routine_even(t_philo *philo)
{
	if (check_end_cond(philo) == PHILO_DIED)
		return ;
	philo_think(philo);
	if (check_end_cond(philo) == PHILO_DIED)
		return ;
	philo_eat(philo);
	if (check_end_cond(philo) == PHILO_DIED)
		return ;
	philo_sleep(philo);
	if (check_end_cond(philo) == PHILO_DIED)
		return ;
}

//check if specific philo reached max meals
static void	run_life(t_philo *philo)
{
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
		pthread_mutex_lock(&philo->philo_mutex);
		if (philo->program->max_meals != MAX_MEALS_DISABLED
			&& philo->meal_number >= philo->program->max_meals)
		{
			pthread_mutex_unlock(&philo->philo_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->philo_mutex);
	}
	if (philo->lfork_status == TAKEN_FORK || philo->rfork_status == TAKEN_FORK)
		release_forks(philo);
}

//init last meal and meal number
//take only one fork
//wait until monitor detects its dead
//release the fork before dying
// when Usleep is released CPU
//at the end relseased fork when program ends
void	*life_one_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	sync_simulation(philo);
	pthread_mutex_lock(&philo->philo_mutex);
	philo->last_meal = precise_time_ms();
	philo->meal_number = 0;
	pthread_mutex_unlock(&philo->philo_mutex);
	// while (check_end_cond(philo) == PHILO_ALIVED)
	// 	usleep(1000);
	pthread_mutex_lock(&philo->left_fork->fork_mutex);
	set_fork_status(philo, philo->left_fork, TAKEN_FORK);
	print_status(philo, "has taken a fork");
	pthread_mutex_unlock(&philo->left_fork->fork_mutex);
	while (check_end_cond(philo) == PHILO_ALIVED)
		usleep(1000);
	set_fork_status(philo, philo->left_fork, NO_TAKEN);
	return (NULL);
}

//each will be a status
//eating, thinking, and sleeping.
//prototype given by pthread_create: void *(*start_routine)(void *), void *arg
//this function handles the thread logic
//odd philos with diff routine than even
//After a full routine, check if max_meals is reached for this philo
// usleep (100) is to Yield CPU to avoid busy-waiting
//steps: sync imulation, set initial meal and be sure meal num is 0
//This happens *after* all threads are ready and 'start_time' is reliable.
//It's essential for the monitor to accurately track hunger from the beginning.
//We lock the philo_mutex to safely update last_meal and meal_number.
//Sync =  Initial delay for even-ID philosophers to prevent immediate deadlock
void	*life_cycle(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	sync_simulation(philo);
	pthread_mutex_lock(&philo->philo_mutex);
	philo->last_meal = precise_time_ms();
	philo->meal_number = 0;
	pthread_mutex_unlock(&philo->philo_mutex);
	usleep(100);
	run_life(philo);
	return (NULL);
}
