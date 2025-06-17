/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 07:42:05 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/17 11:41:56 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
		time_think = philo->program->time_eat;
	think_ends = precise_time_ms() + time_think;
	while (precise_time_ms() < think_ends)
	{
		sim_status = check_end_cond(philo);
		if (sim_status == PHILO_DIED)
			return ;
		usleep(10);
	}
}

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

static int	take_forks(t_philo *philo)
{
	t_fork	*first_fork;
	t_fork	*second_fork;
	int		status;

	status = SUCCESS;
	order_forks(philo, &first_fork, &second_fork);
	pthread_mutex_lock(&first_fork->fork_mutex);
	set_fork_status(philo, first_fork, TAKEN_FORK);
	print_status(philo, "has taken a fork");
	if (check_end_cond(philo) == PHILO_DIED)
	{
		pthread_mutex_unlock(&first_fork->fork_mutex);
		set_fork_status(philo, first_fork, NO_TAKEN);
		return (ERROR);
	}
	pthread_mutex_lock(&second_fork->fork_mutex);
	set_fork_status(philo, second_fork, TAKEN_FORK);
	print_status(philo, "has taken a fork");
	return (SUCCESS);
}

void	release_forks(t_philo *philo)
{
	if (philo->lfork_status == TAKEN_FORK)
	{
		pthread_mutex_unlock(&philo->left_fork->fork_mutex);
		philo->lfork_status = NO_TAKEN;
	}
	if (philo->rfork_status == TAKEN_FORK)
	{
		pthread_mutex_unlock(&philo->right_fork->fork_mutex);
		philo->rfork_status = NO_TAKEN;
	}
}

void	philo_eat(t_philo *philo)
{
	long long	eat_end_time;
	int			sim_status;

	if (take_forks(philo) == ERROR)
		return ;
	sim_status = check_end_cond(philo);
	if (sim_status == PHILO_DIED)
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
}
