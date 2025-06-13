/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:00:31 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/13 17:46:34 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//init mutex for end_flag
//init mutex for output
//return SUCCESS or ERR_MUTEX
int	init_cross_mutex(t_program *data)//do i have to set attributes for philo?
{
	int	mutex_status;

	mutex_status = handling_mutex_init(&data->output_mutex, &data->out_mut_status, "Failed to init output_mutex\n");
	if (mutex_status != SUCCESS)//check if call clean up
		return (mutex_status);
	mutex_status = handling_mutex_init(&data->end_mutex, &data->end_mutex_status,
		"Failed to init end_flag_mutex\n");
	if (mutex_status != SUCCESS)//check if call clean up before return or where
		return (mutex_status);//before return destrou output mutex
	mutex_status = handling_mutex_init(&data->start_mutex, &data->start_mut_status, "Failed to initi start_mutex\n");
	if (mutex_status != SUCCESS)
		return (mutex_status);
	return (mutex_status);//would be success
}

//init mutex per fork
int	init_forks(t_program *data)
{
	int	i;
	int	status;//new

	i = 0;
	data->fork = (t_fork *)malloc(sizeof(t_fork) * data->total_philo);
	if (!data->fork)
		return (malloc_error());
	while (i < data->total_philo)
	{
		data->fork[i].fork_id = i + 1;//check if can begin in 1 and not increas +1 every time
		data->fork[i].fork_mut_status = MUTEX_NO_INIT;
		status = handling_mutex_init(&data->fork[i].fork_mutex, &data->fork[i].fork_mut_status, "Failed to init fork\n");//new
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
void	fill_each_philo(t_program *data, int philo_id)
{
	int		start_fork;//check
	t_philo	*philo;

	start_fork = philo_id -1;//to make it index
	philo = &data->philo[start_fork];//new
	philo->philo_id = philo_id;
	philo->meal_number = 0;
	philo->last_meal = 0;//new
	philo->program = data;
	philo->left_fork = &data->fork[start_fork];
	philo->right_fork = &data->fork[(start_fork + 1) % data->total_philo];//check
	philo->thread_id = 0;//TO AVOID ERRORS, CHECK IF IT'S OK
}
//initi mutex per philo
int	init_philo(t_program *data)
{
	int	i;
	int	status;

	i = 0;
	data->philo = (t_philo *)malloc (sizeof(t_philo) * data->total_philo);
	if (!data->philo)
		return (malloc_error());//check if smt needs to free or destroy mutex before
	while (i < data->total_philo)
	{
		data->philo[i].mutex_status_phi = MUTEX_NO_INIT;
		status = handling_mutex_init(&data->philo[i].philo_mutex, 
			&data->philo[i].mutex_status_phi, "Failed to init philo mutex\n");
		if (status != SUCCESS)
		{
			clean_up_program(data);
			return (ERR_MUTEX);
		}
		data->philo[i].program = data;//check
        // data->philo->forks_taken = 0;//new
		data->philo[i].lfork_status = NO_TAKEN;//checkk syntax
		data->philo[i].rfork_status = NO_TAKEN;
		fill_each_philo(data, i + 1);//needs to be i + 1 if not becomes -1 in fill each philo
		i++;
	}
    // printf("All philos initialized\n");//test
    //pending include destroy mutex for output and sim_over
	return (SUCCESS);
}

//number of philo arr[0]
//time to die arr[1]
//time to eat arr[2]
//time to sleep arr[3]
//optional: # times each philo should eat arr[4]
//mutex will be initialized later

void	init_program(t_program *data)//check if set default really needed
{
     // Initialize time values from parsed input
	data->total_philo = data->parse->arr[0];
	data->time_die = data->parse->arr[1];
	data->time_eat = data->parse->arr[2];
	data->time_sleep = data->parse->arr[3];
    // Initialize synchronization variables
	// data->start_time = precise_time_ms();//will be init in the main
	data->end_flag = PHILO_ALIVED;
    // Initialize mutex statuses
	data->end_mutex_status = MUTEX_NO_INIT;
	data->start_mut_status = MUTEX_NO_INIT;//new
	data->out_mut_status = MUTEX_NO_INIT;//new
    // Initialize data structures pointers
	data->philo = NULL;
	data->fork = NULL;
    // Set max meals
	if (data->parse->count == 4)
		data->max_meals = MAX_MEALS_DISABLED;
	else
		data->max_meals = data->parse->arr[4];
}
