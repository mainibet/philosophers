/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:00:31 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/16 13:36:57 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//init mutex for end_flag
//init mutex for output
//return SUCCESS or ERR_MUTEX
int	init_cross_mutex(t_program *data)
{
	int	mutex_status;

	mutex_status = handling_mutex_init(&data->output_mutex,
			&data->out_mut_status, "Failed to init output_mutex");
	if (mutex_status != SUCCESS)
		return (mutex_status);
	mutex_status = handling_mutex_init(&data->end_mutex,
			&data->end_mutex_status, "Failed to init end_flag_mutex");
	if (mutex_status != SUCCESS)
		return (mutex_status);
	mutex_status = handling_mutex_init(&data->start_mutex,
			&data->start_mut_status, "Failed to initi start_mutex");
	if (mutex_status != SUCCESS)
		return (mutex_status);
	return (mutex_status);
}

//init mutex per fork
//i + 1 fot the id matches the philo
int	init_forks(t_program *data)
{
	int	i;
	int	status;

	i = 0;
	data->fork = (t_fork *)malloc(sizeof(t_fork) * data->total_philo);
	if (!data->fork)
		return (malloc_error());
	while (i < data->total_philo)
	{
		data->fork[i].fork_id = i + 1;
		data->fork[i].fork_mut_status = MUTEX_NO_INIT;
		status = handling_mutex_init(&data->fork[i].fork_mutex,
				&data->fork[i].fork_mut_status, "Failed to init fork");
		if (status != SUCCESS)
			clean_up_program(data);
		i++;
	}
	return (status);
}

//check if needs to be init the last meal or when
//needs to calculate start_fork
//Like this: Calculate the 0-indexed array position for 
//this philosopher's corresponding fork.
//THREAD_ID NOT NEED TO INIT FOR NOW
//start_fork will help to find forks and philo position
//the starting fork is left
//philo thread init in 0 to avoid garbage in memory
static void	fill_each_philo(t_program *data, int philo_id)
{
	int		start_fork;
	t_philo	*philo;

	start_fork = philo_id -1;
	philo = &data->philo[start_fork];
	philo->philo_id = philo_id;
	philo->meal_number = 0;
	philo->last_meal = 0;
	philo->program = data;
	philo->left_fork = &data->fork[start_fork];
	philo->right_fork = &data->fork[(start_fork + 1) % data->total_philo];
	philo->thread_id = 0;
}

//initi mutex per philo
//fill each philo needs to be i + 1 if not becomes -1 in fill each philo
int	init_philo(t_program *data)
{
	int	i;
	int	status;

	i = 0;
	data->philo = (t_philo *)malloc (sizeof(t_philo) * data->total_philo);
	if (!data->philo)
		return (malloc_error());
	while (i < data->total_philo)
	{
		data->philo[i].mutex_status_phi = MUTEX_NO_INIT;
		status = handling_mutex_init(&data->philo[i].philo_mutex,
				&data->philo[i].mutex_status_phi,
				"Failed to init philo mutex");
		if (status != SUCCESS)
		{
			clean_up_program(data);
			return (ERR_MUTEX);
		}
		data->philo[i].program = data;
		data->philo[i].lfork_status = NO_TAKEN;
		data->philo[i].rfork_status = NO_TAKEN;
		fill_each_philo(data, i + 1);
		i++;
	}
	return (SUCCESS);
}

//number of philo arr[0]
//time to die arr[1]
//time to eat arr[2]
//time to sleep arr[3]
//optional: # times each philo should eat arr[4]
// Initialize time values from parsed input
// Initialize mutex statuses
// Initialize data structures pointers
//mutex will be initialized later
// Set max meals
//data->start time initialized in the main to get the correct time
void	init_program(t_program *data)
{
	data->total_philo = data->parse->arr[0];
	data->time_die = data->parse->arr[1];
	data->time_eat = data->parse->arr[2];
	data->time_sleep = data->parse->arr[3];
	data->sim_status = SIM_STOP;
	data->end_flag = PHILO_ALIVED;
	data->end_mutex_status = MUTEX_NO_INIT;
	data->start_mut_status = MUTEX_NO_INIT;
	data->out_mut_status = MUTEX_NO_INIT;
	data->philo = NULL;
	data->fork = NULL;
	if (data->parse->count == 4)
		data->max_meals = MAX_MEALS_DISABLED;
	else
		data->max_meals = data->parse->arr[4];
}
