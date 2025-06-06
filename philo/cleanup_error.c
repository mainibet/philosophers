/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:02:26 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/06 11:12:56 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//i is where the error happened
//will be usefull to destroy clean-up
int	mutex_fork_error(t_program  *data, int i)//DOES THIS NEED TO UNLOCK IF SOMETHING IS LOCK?
{
	int	j;//new

	print_error_msg("Failed to init fork mutex\n");//PENDING DESTROY MUTEX AFTER THE ERROR
	j = 0;
	while (j < i)//new check if needed here or may be while loop in clean-up function
	{
		pthread_mutex_destroy(&data->fork[j].mutex);//check
		j++;
	}
	// free(data->fork);//check
	// data->fork = NULL;//check sure?
	clean_up_program(data);//new CHECK
	return (ERR_MUTEX);
}//can this function work also for other mutex error?

//destroy mutex: output, fork, philo, monitor
//free: array of philo
//free:array of forks
//free: array of parse
void	clean_up_program(t_program *data)//check if philo dies if forks need to be "released"
{//this function should be made by the main thread
	int	i;//should I include a check in case the simulation is running first stop it?
    //should I include the clean_error for forks?

	i = 0;
	if (data->out_mut_status == MUTEX_INIT)
		pthread_mutex_destroy(&data->output_mutex);//need to includ error check of destroy?
	if (data->end_mutex_status == MUTEX_INIT)
		pthread_mutex_destroy(&data->end_mutex);
	if (data->fork != NULL)
	{
		i = 0;
		while (i < data->total_philo)
		{
			if (data->fork[i].fork_mut_status == MUTEX_INIT)
				pthread_mutex_destroy(&data->fork[i].mutex);//need to includ error check of destroy?
			i++;
		}
		// i = count_arr_elements((void **)data->fork);//need to include any handling error?
		free (data->fork);
		data->fork = NULL;//good practice after freeing
		// free_array((void **)data->fork, i);
	}
	if (data->philo != NULL)
	{
		i = 0;
		while (i < data->total_philo)
		{
			if (data->philo[i].mutex_status_phi == MUTEX_INIT)
				pthread_mutex_destroy(&data->philo[i].philo_mutex);
			i++;
		}
		free_array((void *)data->philo, data->total_philo);
	}
	if (data->parse != NULL && data->parse->arr != NULL)
	{
		i = count_arr_elements((void *)data->parse);
		free_array((void *)data->parse, i);
	}
}
