/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 08:58:21 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/17 11:39:05 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	setup_simulation(t_program *data)
{
	int	status;

	if (!data->end_flag)
		data->end_flag = PHILO_ALIVED;
	status = init_cross_mutex(data);
	if (status != SUCCESS)
		return (status);
	status = init_forks(data);
	if (status != SUCCESS)
		return (status);
	status = init_philo(data);
	if (status != SUCCESS)
		return (status);
	status = start_threads(data);
	if (status != SUCCESS)
		return (status);
	return (SUCCESS);
}

void	sync_simulation(t_philo *philo)
{
	pthread_mutex_lock(&philo->program->start_mutex);
	pthread_mutex_unlock(&philo->program->start_mutex);
}

void	set_fork_status(t_philo *philo, t_fork *fork_ptr, int status)
{
	if (fork_ptr == philo->left_fork)
		philo->lfork_status = status;
	else
		philo->rfork_status = status;
}

void	start_simulation(t_program *data)
{
	data->start_time = precise_time_ms();
	usleep(100);
	pthread_mutex_lock(&data->end_mutex);
	data->sim_status = SIM_RUNNING;
	pthread_mutex_unlock(&data->end_mutex);
	pthread_mutex_unlock(&data->start_mutex);
}

int	check_end_cond(t_philo *philo)
{
	int	sim_status;

	pthread_mutex_lock(&philo->program->end_mutex);
	sim_status = philo->program->end_flag;
	pthread_mutex_unlock(&philo->program->end_mutex);
	return (sim_status);
}
