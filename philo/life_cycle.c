/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_cycle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 07:49:31 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/06 07:59:28 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_routine_odd(t_philo *philo)//check if is static
{
	philo_eat(philo);
	philo_sleep(philo);
	philo_think(philo);
}

static void	philo_routine_even(t_philo *philo)//check if is static
{
	philo_think(philo);
	philo_eat(philo);
	philo_sleep(philo);
}
//takes the dying philo as argument
//set the end_flag as PHILO_DIED (1)
//lock the end_flag
//set global termination flag
//unlock flag
void	kill_philo(t_philo *philo)//check and also if is static
{
	// pthread_mutex_lock(&philo->program->end_mutex);
	// philo->program->end_flag = PHILO_DIED;
	// pthread_mutex_unlock(&philo->program->end_mutex);
	// print_status(philo, "died");//after this call the clean_up
	end_program(philo->program, "died", philo);
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
void	*life_cycle(void *arg)
{
	t_philo	*philo;
	int		wait_time;

	philo = (t_philo *)arg;
	if (philo->philo_id % 2 == 0)//even small delay CHECK
	{
		wait_time = philo->program->time_eat;//check
		usleep(wait_time * 1000);
	}
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
	return (NULL);
}
