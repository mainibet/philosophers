/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 08:58:21 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/10 13:45:49 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_end_cond(t_philo *philo)
{
	int	sim_status;

	pthread_mutex_lock(&philo->program->end_mutex);
	sim_status = philo->program->end_flag;
	pthread_mutex_unlock(&philo->program->end_mutex);
	return (sim_status);
}

//sim wait until all threads joined and clean-up
// void	sim_stop(t_program *data)
// {
// 	int	i;

// 	i = 0;
// 	while (i < data->total_philo)
// 	{
// 		pthread_join(data->philo[i].thread_id, NULL);//check
// 		i++;
// 	}
// 	if (data->total_philo > 0)
// 	{
// 		if (data->monitor_thread_id != 0)
// 			pthread_join(data->monitor_thread_id, NULL);//check
// 	}
// }

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
