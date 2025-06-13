/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:02:26 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/13 17:45:58 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//This function will ONLY destroy fork mutexes.
static void	clean_mutex_forks(t_program *data)//check if the i isn eeded from before
{
	int	i;

	i = 0;
	while (i < data->total_philo)
	{
		if (data->fork[i].fork_mut_status == MUTEX_INIT)
			pthread_mutex_destroy(&data->fork[i].fork_mutex);//need to includ error check of destroy?
		i++;
	}
}

//This function will ONLY destroy philo mutexes.
static void	clean_mutex_philo(t_program *data)
{
	int	i;

	i = 0;
	while (i < data->total_philo)
	{
		if (data->philo[i].mutex_status_phi == MUTEX_INIT)
			pthread_mutex_destroy(&data->philo[i].philo_mutex);
		i++;
	}
}

static void destroy_global_mutex(t_program *data)//new
{
	if (data->out_mut_status == MUTEX_INIT)
		pthread_mutex_destroy(&data->output_mutex);//need to includ error check of destroy?
	if (data->end_mutex_status == MUTEX_INIT)
		pthread_mutex_destroy(&data->end_mutex);
	if (data->start_mut_status == MUTEX_INIT)
		pthread_mutex_destroy(&data->start_mutex);
	return ;
}

//destroy mutex: output, fork, philo, monitor
//free: array of philo
//free:array of forks
//free: array of parse is in the heap but parse struct is in the stack
void	clean_up_program(t_program *data)//check if philo dies if forks need to be "released"
{//this function should be made by the main thread
	int	i;
    //should I include the clean_error for forks?
//should I include a check in case the simulation is running first stop it?
	i = 0;//include pthread_mutex_unlock(&data.start_mutex);
	destroy_global_mutex(data);
	if (data->fork != NULL)
	{
		clean_mutex_forks(data);//new
		free(data->fork);
		data->fork = NULL; // Good practice: nullify after freeing
	}
	if (data->philo != NULL)//new
	{
		clean_mutex_philo(data);
		free(data->philo);
		data->philo = NULL;
	}
	if (data->parse != NULL)//check if the struct no need to be freed
	{
		if (data->parse->arr != NULL)//check the array inside
		{
			free(data->parse->arr);//free arr
			data->parse->arr = NULL;//check if is good
		}
	}
	if (data != NULL)//new this cond
	// {
		free(data);
		// data = NULL;
	// }
}

int	malloc_error(void)//use write instead of printf
{
	print_error_msg("Memory allocation failed");//check if smt to free
	return (ERR_MALLOC_FAIL);
}
