/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 07:35:32 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/13 16:59:17 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//HELPER FUNCTION
//to print all the msg in order
//lock output mutex
//lock end flag mutex
//check end flag
//unlock end flag mutex
//if end flag is ok then print status
//unlock print status (output)
//it (!sim-status) means is 0 (PHILO_ALIVED)
//only for life status
void	print_status(t_philo *philo, const char *msg)//check if needs to handled any error
{
	long long	current_time;
	int			sim_status;

	pthread_mutex_lock(&philo->program->start_mutex);
	current_time = precise_time_ms() - philo->program->start_time;
	sim_status = check_end_cond(philo);
	if (sim_status == PHILO_ALIVED)
	{
		pthread_mutex_lock(&philo->program->output_mutex);
		printf("%lld %d %s\n", current_time, philo->philo_id, msg);
		pthread_mutex_unlock(&philo->program->output_mutex);
	}
	pthread_mutex_unlock(&philo->program->start_mutex);//check if here or where
}

int	ft_strlen(const char *str)
{
	int	count;

	count = 0;    
	while (str[count])
		count++;
	return (count);
}
//not needed mutex here
void	print_error_msg(const char *msg)
{
	const char	*prefix;

	prefix = "Error: ";   
	write(STDERR_FILENO, prefix, ft_strlen(prefix));
	write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 1);
	// printf("Error: %s\n", msg);
}
