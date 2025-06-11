/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_cycle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 07:49:31 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/11 12:38:18 by albetanc         ###   ########.fr       */
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
//To wait for simulation start signal
// Thread Synchronization Barrier
void sync_simulation(t_philo *philo)//MOVE TO OTHER FILE
{
	pthread_mutex_lock(&philo->program->start_mutex);
	pthread_mutex_unlock(&philo->program->start_mutex);
}

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
		pthread_mutex_lock(&philo->philo_mutex);//check if specific philo reached max meals
		if (philo->program->max_meals != MAX_MEALS_DISABLED
			&& philo->meal_number >= philo->program->max_meals)
		{
			pthread_mutex_unlock(&philo->philo_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->philo_mutex);
	}
}
//init last meal and meal number
//take only one fork
//wait until monitor detects its dead
//release the fork before dying
static void	*life_one_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	sync_simulation(philo);
	pthread_mutex_lock(&philo->philo_mutex);
	philo->last_meal = precise_time_ms();
	philo->meal_number = 0;
	pthread_mutex_unlock(&philo->philo_mutex);
	pthread_mutex_lock(&philo->left_fork->mutex);
	print_status(philo, "has taken a fork");
	while (check_end_cond(philo) == PHILO_ALIVED)
		usleep(1000); // release CPU
	pthread_mutex_unlock(&philo->left_fork->mutex); // Libera el tenedor al morir/terminar
	return (NULL);
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
//2.  Initialize philosopher's state after global start time is set 
//This happens *after* all threads are ready and 'start_time' is reliable.
//It's essential for the monitor to accurately track hunger from the beginning.
//We lock the philo_mutex to safely update last_meal and meal_number.
//3. Initial delay for even-ID philosophers to prevent immediate deadlock
void	*life_cycle(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	sync_simulation(philo);
	pthread_mutex_lock(&philo->philo_mutex);//init philos
	philo->last_meal = precise_time_ms(); // Set initial last_meal to global simulation start time
	philo->meal_number = 0; // Ensure meal_number is 0 at the start of its life cycle
	pthread_mutex_unlock(&philo->philo_mutex);//end init philos
	usleep(100);
	run_life(philo);
	return (NULL);
}
