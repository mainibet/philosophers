/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:02:26 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/17 11:41:02 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	destroy_global_mutex(t_program *data)
{
	if (data->out_mut_status == MUTEX_INIT)
		pthread_mutex_destroy(&data->output_mutex);
	if (data->end_mutex_status == MUTEX_INIT)
		pthread_mutex_destroy(&data->end_mutex);
	if (data->start_mut_status == MUTEX_INIT)
		pthread_mutex_destroy(&data->start_mutex);
	return ;
}

void	clean_up_program(t_program *data)
{
	if (!data)
		return ;
	destroy_global_mutex(data);
	clean_fork(data);
	clean_philo(data);
	clean_parse(data);
	free(data);
}

int	malloc_error(void)
{
	print_error_msg("Memory allocation failed");
	return (ERR_MALLOC_FAIL);
}

int	handle_creation_error(t_program *data, int status)
{
	pthread_mutex_unlock(&data->start_mutex);
	return (status);
}
