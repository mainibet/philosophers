/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 07:35:32 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/17 08:20:04 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//to print all the msg in order
//lock & unlock mutex: start_time, enf_flag, output
void	print_status(t_philo *philo, const char *msg)
{
	long long	current_time;
	int			sim_status;

	pthread_mutex_lock(&philo->program->end_mutex);//new
	sim_status = philo->program->end_flag;//new
	pthread_mutex_unlock(&philo->program->end_mutex);//new
	// pthread_mutex_lock(&philo->program->start_mutex);
	// pthread_mutex_lock(&philo->program->output_mutex);
	// current_time = precise_time_ms() - philo->program->start_time;
	// sim_status = check_end_cond(philo);
	if (sim_status == PHILO_ALIVED)
	{
		pthread_mutex_lock(&philo->program->start_mutex);//relocated
		pthread_mutex_lock(&philo->program->output_mutex);//relocated
		current_time = precise_time_ms() - philo->program->start_time;//relocated
		printf("%lld %d %s\n", current_time, philo->philo_id, msg);
		pthread_mutex_unlock(&philo->program->output_mutex);
		pthread_mutex_unlock(&philo->program->start_mutex);
 	}
}

//not needed mutex for error output
void	print_error_msg(const char *msg)
{
	const char	*prefix;

	prefix = "Error: ";
	write(STDERR_FILENO, prefix, ft_strlen(prefix));
	write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 1);
}
