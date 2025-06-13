/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 08:58:21 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/13 17:10:29 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	setup_simulation(t_program *data)//check if **argv needed or only data?
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

//To wait for simulation start signal
// Thread Synchronization Barrier
void sync_simulation(t_philo *philo)
{
	pthread_mutex_lock(&philo->program->start_mutex);
	pthread_mutex_unlock(&philo->program->start_mutex);
}

//helper function
void	set_fork_status(t_philo *philo, t_fork *fork_ptr, int status)
{
	if (fork_ptr == philo->left_fork)
		philo->lfork_status = status;
	else
		philo->rfork_status = status;
}

int	check_end_cond(t_philo *philo)
{
	int	sim_status;

	pthread_mutex_lock(&philo->program->end_mutex);
	sim_status = philo->program->end_flag;
	pthread_mutex_unlock(&philo->program->end_mutex);
	return (sim_status);
}

void	end_program(t_program *data, char *msg, t_philo *dying_philo)//this should be kill? and check
{
	long long	current_time;

	pthread_mutex_lock(&data->end_mutex);
	if (data->end_flag == PHILO_ALIVED) // Only set if not already terminated
	{
		data->end_flag = PHILO_DIED;
		pthread_mutex_lock(&data->output_mutex);
		current_time = precise_time_ms() - data->start_time;//new
		if (dying_philo)
			printf("%lld %d %s\n", current_time, dying_philo->philo_id, msg);//new
		else
			printf("%lld %s\n", current_time, msg);//new CHECK
		pthread_mutex_unlock(&data->output_mutex);
	}
	pthread_mutex_unlock(&data->end_mutex);
}
