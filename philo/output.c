/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 07:35:32 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/06 07:41:05 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_format(t_philo *philo, const char *msg)//will be used?
{
	long long	current_time;

	current_time = precise_time_ms() - philo->program->start_time;
	printf("%lld %d %s\n", current_time, philo->philo_id, msg);
}

//HELPER FUNCTION
//to print all the msg in order
//lock output mutex
//lock end flag mutex
//check end flag
//unlock end flag mutex
//if end flag is ok then print status
//unlock print status (output)
//it (!sim-status) means is 0 (PHILO_ALIVED)
void	print_status(t_philo *philo, const char *msg)//check if needs to handled any error
{
	long long	current_time;
	int			sim_status;//is same as philo_died or Alived

	current_time = precise_time_ms() - philo->program->start_time;
	// pthread_mutex_lock(&philo->program->output_mutex);
	sim_status = check_end_cond(philo);
	if (sim_status == PHILO_ALIVED)
	{
		pthread_mutex_lock(&philo->program->output_mutex);
		printf("%lld %d %s\n", current_time, philo->philo_id, msg);
		pthread_mutex_unlock(&philo->program->output_mutex);
	}
}

//pending destroy mytex if an error occure after init mutex
void	print_error_msg(const char *msg)
{
	printf("Error: %s\n", msg);
}
